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
}

void UPlayableCharacterCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(PlayableCharacter == nullptr)	return;
	
	if(EquippedWeapon)
	{
		// TODO : change weapon, not destroy
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetOwner(PlayableCharacter);
	const auto RightHandSocket = PlayableCharacter->GetMesh()->GetSocketByName("hand_r_weapon_socket");
	if(RightHandSocket)
	{
		RightHandSocket->AttachActor(EquippedWeapon, PlayableCharacter->GetMesh());
	}
}

void UPlayableCharacterCombatComponent::Attack()
{
}
