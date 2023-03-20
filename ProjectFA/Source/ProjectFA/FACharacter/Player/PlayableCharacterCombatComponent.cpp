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
		DefaultPunchWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultPunchWeaponClass);
		EquipWeapon(DefaultPunchWeapon);
	}
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
	if(const auto RightHandSocket = PlayableCharacter->GetMesh()->GetSocketByName("hand_r_weapon_socket"))
	{
		RightHandSocket->AttachActor(EquippedWeapon, PlayableCharacter->GetMesh());
	}
}

void UPlayableCharacterCombatComponent::Attack() const
{
	if(EquippedWeapon == nullptr)	return;
	PlayableCharacter->PlayNormalAttackMontage(AWeapon::GetWeaponSectionName(EquippedWeapon));
}
