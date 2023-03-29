// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayableCharacter.h"
#include "PlayableCharacterCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectFA/InGameItem/Equipable.h"
#include "ProjectFA/InGameItem/PickupItem.h"

UInventoryComponent::UInventoryComponent()
	:
	InventoryItemTotalWeight(0.f)
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

void UInventoryComponent::SetNearbyItemToInventory()
{
	if(NearbyItemList.IsEmpty())	return;

	APickupItem* ItemToGetIn = NearbyItemList[0];
	AddItemToInventory(ItemToGetIn);
}

void UInventoryComponent::RemoveItem(APickupItem* Item)
{
	Item->SetItemState(EItemState::EIS_MAX);
	Item->SetOwner(nullptr);
	DeleteItemFromInventory(Item);
}

void UInventoryComponent::AddInventoryWeight(const float& ItemWeightToIn)
{
	InventoryItemTotalWeight += ItemWeightToIn;
	InventoryWeightChangedEvent.Broadcast(InventoryItemTotalWeight);
}

void UInventoryComponent::SubtractInventoryWeight(const float& ItemWeightToOut)
{
	InventoryItemTotalWeight -= ItemWeightToOut;
	InventoryWeightChangedEvent.Broadcast(InventoryItemTotalWeight);
}

void UInventoryComponent::AddItemToInventory(APickupItem* ItemToIn)
{
	ItemToIn->SetOwner(GetOwner());
	ItemToIn->SetItemState(EItemState::EIS_InInventory);
	ItemToIn->ItemRemovedFromInventoryEvent.AddDynamic(this, &UInventoryComponent::RemoveItem);
	InventoryItemList.Add(ItemToIn);
	DeleteNearbyItem(ItemToIn);
	InventoryChangedEvent.Broadcast(InventoryItemList);
	AddInventoryWeight(ItemToIn->GetItemWeight());

		
	if(UKismetSystemLibrary::DoesImplementInterface(ItemToIn, UEquipable::StaticClass()))
	{
		ItemToIn->SetItemState(EItemState::EIS_Equipped);
		PlayerCombatComponent->EquipItemToCharacter(ItemToIn);
	}
}

void UInventoryComponent::DeleteItemFromInventory(APickupItem* ItemToOut)
{
	InventoryItemList.Remove(ItemToOut);
	InventoryChangedEvent.Broadcast(InventoryItemList);
	SubtractInventoryWeight(ItemToOut->GetItemWeight());
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