// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayableCharacter.h"
#include "PlayableCharacterCombatComponent.h"
#include "ProjectFA/InGameItem/Weapon/Weapon.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayableCharacter = Cast<APlayableCharacter>(GetOwner());
	if(PlayableCharacter)
	{
		PlayerCombatComponent = PlayableCharacter->GetplayerCombatComponent();
	}
}

void UInventoryComponent::SetPickupItemToInventory(APickupItem* InteractableItem)
{
	if(const auto Weapon = Cast<AWeapon>(InteractableItem))
	{
		PlayerCombatComponent->EquipWeapon(Weapon);
	}
}