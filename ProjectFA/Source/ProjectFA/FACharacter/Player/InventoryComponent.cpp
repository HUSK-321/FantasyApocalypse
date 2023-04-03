// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayableCharacter.h"
#include "PlayableCharacterCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectFA/InGameItem/Equipable.h"
#include "ProjectFA/InGameItem/PickupItem.h"

UInventoryComponent::UInventoryComponent()
	:
	InventoryCapacity(15), InventoryItemTotalWeight(0.f)
{
	PrimaryComponentTick.bCanEverTick = false;

	InventoryItemList.Reserve(InventoryCapacity);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(const auto PlayableCharacter = Cast<ICombatableCharacter>(GetOwner()))
	{
		PlayerCombatComponent = Cast<UPlayableCharacterCombatComponent>(PlayableCharacter->GetCombatComponent());
	}
}

void UInventoryComponent::SetNearbyItemToInventory()
{
	if(NearbyItemList.IsEmpty() || InventoryItemList.Num() >= InventoryCapacity)	return;
	APickupItem* ItemToGetIn = NearbyItemList[0];
	AddItemToInventory(ItemToGetIn);
}

void UInventoryComponent::AddItemToInventory(APickupItem* ItemToIn)
{	
	ItemToIn->SetOwner(GetOwner());
	ItemToIn->SetItemState(EItemState::EIS_InInventory);
	ItemToIn->ItemDroppedEvent.AddDynamic(this, &UInventoryComponent::DropItemFromInventory);
	InventoryItemList.Add(ItemToIn);
	DeleteNearbyItem(ItemToIn);
	InventoryChangedEvent.Broadcast(InventoryItemList);
	AddInventoryWeight(ItemToIn->GetItemWeight());
		
	if(auto EquipableItem = Cast<IEquipable>(ItemToIn))
	{
		FEquipItemEvent EquipItemEvent;
		EquipItemEvent.AddDynamic(this, &UInventoryComponent::EquipItem);
		EquipableItem->SetEquipItemEvent(EquipItemEvent);
	}
}

void UInventoryComponent::DropItemFromInventory(APickupItem* ItemToOut)
{
	ItemToOut->SetOwner(nullptr);
	InventoryItemList.Remove(ItemToOut);
	InventoryChangedEvent.Broadcast(InventoryItemList);
	SubtractInventoryWeight(ItemToOut->GetItemWeight());
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

void UInventoryComponent::AddNearbyItem(AActor* Item)
{
	const auto PickupItemActor = Cast<APickupItem>(Item);
	if(PickupItemActor == nullptr)	return;
	NearbyItemList.Add(PickupItemActor);
	NearbyItemAddEvent.Broadcast(PickupItemActor);
}

void UInventoryComponent::DeleteNearbyItem(AActor* Item)
{
	const auto PickupItemActor = Cast<APickupItem>(Item);
	if(PickupItemActor == nullptr)	return;
	NearbyItemList.Remove(PickupItemActor);
	NearbyItemDeleteEvent.Broadcast(PickupItemActor);
}

void UInventoryComponent::EquipItem(APickupItem* Item)
{
	PlayerCombatComponent->EquipItemToCharacter(Item);
}