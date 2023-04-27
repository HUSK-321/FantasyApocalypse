// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "WeaponItemDataAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void AWeapon::SetItemPropertyFromDataAsset(const UItemDataAsset* DataAsset)
{
	Super::SetItemPropertyFromDataAsset(DataAsset);
	const auto WeaponDataAsset = Cast<UWeaponItemDataAsset>(DataAsset);
	if(WeaponDataAsset == nullptr)	return;

	UE_LOG(LogTemp, Warning, TEXT("set weapon data"));
	WeaponMesh->SetSkeletalMesh(WeaponDataAsset->WeaponSkeletalMesh);
	WeaponType = WeaponDataAsset->WeaponType;
	DamageTypeClass = WeaponDataAsset->DamageTypeClass;
}

void AWeapon::SetItemState(const EItemState State)
{
	Super::SetItemState(State);

	switch (State)
	{
	case EItemState::EIS_Initial:
		WeaponMesh->SetVisibility(false);
		break;
		
	case EItemState::EIS_Equipped:
		UE_LOG(LogTemp, Warning, TEXT("set weapon visibility true in location : %s"), *WeaponMesh->GetComponentLocation().ToString());
		WeaponMesh->SetVisibility(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_InInventory:
		WeaponMesh->SetVisibility(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_Dropped:
		WeaponMesh->SetVisibility(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	default:
		break;
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

FName AWeapon::GetNormalAttackMontageSectionName() const
{
	switch (WeaponType)
	{
	case EWeaponType::EWT_OneHandSword:
		return FName(TEXT("OneHandSword"));
	case EWeaponType::EWT_MagicStaff:
		return FName(TEXT("MagicStaff"));
	default:
		return FName(TEXT("Default"));
	}
}

void AWeapon::UnEquip()
{
	const FDetachmentTransformRules DetachRules{ EDetachmentRule::KeepWorld, true };
	WeaponMesh->DetachFromComponent(DetachRules);
	SetItemState(EItemState::EIS_InInventory);
	UnEquipEvent.Broadcast(this);
}

void AWeapon::WeaponAttacking_Implementation()
{
	if(DamageTypeClass == nullptr)	return;
	
	HittedActors.AddUnique(GetOwner());
	HittedActors.AddUnique(this);
	const FVector TraceStart{ WeaponMesh->GetSocketLocation(TEXT("AttackCapsuleStart")) };
	const FVector TraceEnd{ WeaponMesh->GetSocketLocation(TEXT("AttackCapsuleEnd")) };
	const float CapsuleRadius = 5.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) };
	TArray<FHitResult> OutHits;

	// TODO : PickupItemMesh 이후 변경
	UKismetSystemLibrary::SphereTraceMultiForObjects(WeaponMesh, TraceStart, TraceEnd, CapsuleRadius, ObjectTypes, false, HittedActors,
														EDrawDebugTrace::ForDuration, OutHits, true);

	for(const auto HitResult : OutHits)
	{
		if(HitResult.bBlockingHit == false)	continue;
		const APawn* AttackingPawn = Cast<APawn>(GetOwner());
		if(AttackingPawn == nullptr)	continue;
		const auto AttackingInstigator = AttackingPawn->GetController();
		if(AttackingInstigator == nullptr)	continue;
		if(HittedActors.Contains(HitResult.GetActor()))	continue;
	
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), ItemPowerAmount, AttackingInstigator, this, DamageTypeClass);
		HittedActors.AddUnique(HitResult.GetActor());
	}
}

void AWeapon::AttackEnd_Implementation()
{
	HittedActors.Empty();
}

void AWeapon::SetEquipItemEvent(const FEquipItemEvent& Event)
{
	EquipItemEvent.Clear();
	EquipItemEvent = Event;
}

void AWeapon::SetUnEquipEvent(const FEquipItemEvent& Event)
{
	UnEquipEvent = Event;	
}

void AWeapon::InventoryAction_Implementation()
{
	EquipItemEvent.Broadcast(this);
}

void AWeapon::RemoveFromInventoryAction_Implementation()
{
	UnEquip();
	DropItem();
}