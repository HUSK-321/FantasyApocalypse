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

void UInventoryComponent::AddNearbyItem(APickupItem* Item)
{
	NearbyItemList.Add(Item);
	NearbyItemAddEvent.Broadcast(Item);
}

void UInventoryComponent::DeleteNearbyItem(APickupItem* Item)
{
	NearbyItemList.Remove(Item);
	NearbyItemDeleteEvent.Broadcast(Item);
}

void UInventoryComponent::SetNearbyItemToInventory()
{
	if(NearbyItemList.IsEmpty())	return;

	APickupItem* ItemToGetIn = NearbyItemList[0];
	ItemToGetIn->SetOwner(GetOwner());
	ItemToGetIn->SetItemState(EItemState::EIS_InInventory);
	ItemToGetIn->ItemRemovedEvent.AddDynamic(this, &UInventoryComponent::RemoveItem);
	InventoryItemList.Add(ItemToGetIn);
	DeleteNearbyItem(ItemToGetIn);
	
	InventoryChangedEvent.Broadcast(InventoryItemList);
	
	if(UKismetSystemLibrary::DoesImplementInterface(ItemToGetIn, UEquipable::StaticClass()))
	{
		ItemToGetIn->SetItemState(EItemState::EIS_Equipped);
		PlayerCombatComponent->EquipItemToCharacter(ItemToGetIn);
	}
}

void UInventoryComponent::RemoveItem(APickupItem* Item)
{
	Item->SetItemState(EItemState::EIS_MAX);
	Item->SetOwner(nullptr);
	InventoryItemList.Remove(Item);
	InventoryChangedEvent.Broadcast(InventoryItemList);
}