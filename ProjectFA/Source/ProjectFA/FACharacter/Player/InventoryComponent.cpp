// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayableCharacter.h"
#include "PlayableCharacterCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectFA/InGameItem/Equipable.h"
#include "ProjectFA/InGameItem/PickupItem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if(const auto PlayableCharacter = Cast<APlayableCharacter>(GetOwner()))
	{
		PlayerCombatComponent = PlayableCharacter->GetPlayerCombatComponent();
	}
}

void UInventoryComponent::GetItemToInventory(APickupItem* InteractableItem)
{
	InteractableItem->SetItemState(EItemState::EIS_InInventory);
	if(UKismetSystemLibrary::DoesImplementInterface(InteractableItem, UEquipable::StaticClass()))
	{
		InteractableItem->SetItemState(EItemState::EIS_Equipped);
		PlayerCombatComponent->EquipItemToCharacter(InteractableItem);
	}
}