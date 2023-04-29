// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "WeaponItemDataAsset.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetRootComponent());
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackCollision->bHiddenInGame = false;
}

void AWeapon::SetItemPropertyFromDataAsset(const UItemDataAsset* DataAsset)
{
	Super::SetItemPropertyFromDataAsset(DataAsset);
	const auto WeaponDataAsset = Cast<UWeaponItemDataAsset>(DataAsset);
	if(WeaponDataAsset == nullptr)	return;

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

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_Equipped:
		WeaponMesh->SetVisibility(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_InInventory:
		WeaponMesh->SetVisibility(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_Dropped:
		WeaponMesh->SetVisibility(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	default:
		break;
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::AttackCollisionOnOverlapBegin);
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
	this->DetachFromActor(DetachRules);
	SetItemState(EItemState::EIS_InInventory);
	UnEquipEvent.Broadcast(this);
}

void AWeapon::AttackStart_Implementation()
{
	HittedActors.AddUnique(GetOwner());
	HittedActors.AddUnique(this);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AWeapon::AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(DamageTypeClass == nullptr || HittedActors.Contains(OtherActor) || DamageTypeClass == nullptr)	return;
	
	const APawn* AttackingPawn = Cast<APawn>(GetOwner());
	if(AttackingPawn == nullptr)	return;
	const auto AttackingInstigator = AttackingPawn->GetController();
	if(AttackingInstigator == nullptr)	return;
	
	UGameplayStatics::ApplyDamage(OtherActor, ItemPowerAmount, AttackingInstigator, this, DamageTypeClass);
	HittedActors.AddUnique(OtherActor);

	if(GetWorld())
	{
		const FVector DebugLineStart = SweepResult.ImpactPoint;
		const FVector DebugLineEnd = DebugLineStart + SweepResult.ImpactNormal * 10.f;
		UE_LOG(LogTemp, Warning, TEXT("impact normal : %s"), *SweepResult.ImpactNormal.ToString());
		UE_LOG(LogTemp, Warning, TEXT("start : %s, end : %s"), *DebugLineStart.ToString(), *DebugLineEnd.ToString());
		DrawDebugLine(GetWorld(), DebugLineStart, DebugLineEnd, FColor::Cyan, false, 20.f, 0, 1.f);	
	}
}

void AWeapon::AttackEnd_Implementation()
{
	HittedActors.Empty();
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::SetEquipItemEvent(const FEquipItemEvent& Event)
{
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