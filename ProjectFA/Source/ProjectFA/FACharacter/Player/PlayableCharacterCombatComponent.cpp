// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacterCombatComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "ProjectFA/InGameItem/Weapon/Weapon.h"

UPlayableCharacterCombatComponent::UPlayableCharacterCombatComponent()
	:
	bNowAttacking(false), bShouldStopAttack(false)
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

	if(const auto Equipable = Cast<IEquipable>(ItemToEquip))
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
}

void UPlayableCharacterCombatComponent::Attack()
{
	if(EquippedItem == nullptr)	return;
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		bNowAttacking = true;
		Character->PlayNormalAttackMontage(WeaponInterface->GetNormalAttackMontageSectionName());
	}
}

void UPlayableCharacterCombatComponent::ShouldStopAttack()
{
	bShouldStopAttack = true;
}

void UPlayableCharacterCombatComponent::CheckShouldStopAttack()
{
	if(bShouldStopAttack == false) return;

	bNowAttacking = false;
	bShouldStopAttack = false;
	Character->StopNormalAttackMontage();
}

void UPlayableCharacterCombatComponent::SetWeaponAttackCollision(bool bEnabled)
{
	if(EquippedItem == nullptr)	return;
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		WeaponInterface->SetAttackCollision(bEnabled);
	}
}
