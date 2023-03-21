// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacterCombatComponent.h"
#include "PlayableCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ProjectFA/InGameItem/Weapon/Weapon.h"

UPlayableCharacterCombatComponent::UPlayableCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayableCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayableCharacter = Cast<APlayableCharacter>(GetOwner());
	if(DefaultPunchWeaponClass)
	{
		DefaultPunchWeapon = GetWorld()->SpawnActor<APickupItem>(DefaultPunchWeaponClass);
		EquipItemToCharacter(DefaultPunchWeapon);
	}
}

void UPlayableCharacterCombatComponent::EquipItemToCharacter(APickupItem* ItemToEquip)
{
	if(PlayableCharacter == nullptr)	return;
	
	if(EquippedItem)
	{
		// TODO : change weapon, not destroy
		EquippedItem->Destroy();
	}
	EquippedItem = ItemToEquip;
	EquippedItem->SetOwner(PlayableCharacter);
	if(const auto RightHandSocket = PlayableCharacter->GetMesh()->GetSocketByName("hand_r_weapon_socket"))
	{
		RightHandSocket->AttachActor(EquippedItem, PlayableCharacter->GetMesh());
	}
}

void UPlayableCharacterCombatComponent::Attack() const
{
	if(EquippedItem == nullptr)	return;
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		PlayableCharacter->PlayNormalAttackMontage(WeaponInterface->GetNormalAttackMontageSectionName());	
	}
}

void UPlayableCharacterCombatComponent::SetWeaponAttackCollision(bool bEnabled)
{
	if(EquippedItem == nullptr)	return;
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		WeaponInterface->SetAttackCollision(bEnabled);
	}
}
