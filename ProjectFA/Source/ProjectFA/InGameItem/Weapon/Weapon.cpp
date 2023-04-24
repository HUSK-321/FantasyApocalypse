// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
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
	PickupItemMesh->DetachFromComponent(DetachRules);
	SetItemState(EItemState::EIS_InInventory);
	UnEquipEvent.Broadcast(this);
}

void AWeapon::WeaponAttacking_Implementation()
{
	HittedActors.AddUnique(GetOwner());
	HittedActors.AddUnique(this);
	const FVector TraceStart{ PickupItemMesh->GetSocketLocation(TEXT("AttackCapsuleStart")) };
	const FVector TraceEnd{ PickupItemMesh->GetSocketLocation(TEXT("AttackCapsuleEnd")) };
	const float CapsuleRadius = 20.f;
	const float CapsuleHeight = 10.f;
	FHitResult HitResult;
	
	UKismetSystemLibrary::CapsuleTraceSingle(this, TraceStart, TraceEnd, CapsuleRadius, CapsuleHeight, UEngineTypes::ConvertToTraceType(ECC_Pawn),
									false, HittedActors, EDrawDebugTrace::ForDuration, HitResult, true);
	if(HitResult.bBlockingHit)
	{
		const APawn* AttackingPawn = Cast<APawn>(GetOwner());
		if(AttackingPawn == nullptr || DamageTypeClass == nullptr)	return;
		const auto AttackingInstigator = AttackingPawn->GetController();
		if(AttackingInstigator == nullptr)	return;
		
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