// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacterCombatComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "ProjectFA/InGameItem/Weapon/Weapon.h"

UPlayableCharacterCombatComponent::UPlayableCharacterCombatComponent()
	:
	bNowAttacking(false), bDoNextAttack(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayableCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AFACharacter>(GetOwner());
	if(DefaultPunchWeaponClass)
	{
		DefaultPunchWeapon = GetWorld()->SpawnActor<APickupItem>(DefaultPunchWeaponClass);
		EquipItemToCharacter(DefaultPunchWeapon);
	}
}

void UPlayableCharacterCombatComponent::EquipItemToCharacter(APickupItem* ItemToEquip)
{
	if(Character == nullptr)	return;
	if(const auto Equipable = Cast<IEquipable>(EquippedItem))
	{
		Equipable->UnEquip();
	}
	EquippedItem = ItemToEquip;
	EquippedItem->SetOwner(Character);
	EquippedItem->SetItemState(EItemState::EIS_Equipped);
	if(const auto RightHandSocket = Character->GetMesh()->GetSocketByName("hand_r_weapon_socket"))
	{
		RightHandSocket->AttachActor(EquippedItem, Character->GetMesh());
	}
	if(const auto Equipable = Cast<IEquipable>(ItemToEquip))
	{
		FEquipItemEvent Event;
		Event.AddDynamic(this, &UPlayableCharacterCombatComponent::ItemDrop);
		Equipable->SetUnEquipEvent(Event);
	}
}

void UPlayableCharacterCombatComponent::Attack()
{
	if(EquippedItem == nullptr || Character == nullptr)	return;
	if(bNowAttacking)
	{
		bDoNextAttack = true;
		return;
	}
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		bNowAttacking = true;
		Character->PlayNormalAttackMontage(WeaponInterface->GetNormalAttackMontageSectionName());
	}
}

void UPlayableCharacterCombatComponent::WeaponAttacking()
{
	if(EquippedItem == nullptr)	return;
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		WeaponInterface->WeaponAttacking_Implementation();
	}
}

void UPlayableCharacterCombatComponent::WeaponAttackEnd()
{
	if(EquippedItem == nullptr)	return;
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		WeaponInterface->AttackEnd_Implementation();
		bDoNextAttack = false;
	}
}

void UPlayableCharacterCombatComponent::CheckShouldStopAttack()
{
	if(bDoNextAttack || Character == nullptr)
	{
		bDoNextAttack = false;
		return;
	}
	bNowAttacking = false;
	Character->StopNormalAttackMontage();
}

void UPlayableCharacterCombatComponent::ItemDrop(APickupItem* UnEquipItem)
{
	if(UnEquipItem == EquippedItem)
	{
		EquippedItem = nullptr;
	}
}