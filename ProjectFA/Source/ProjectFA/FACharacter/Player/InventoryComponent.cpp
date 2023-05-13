// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayableCharacter.h"
#include "PlayableCharacterCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectFA/InGameItem/Equipable.h"
#include "ProjectFA/InGameItem/PickupItem.h"

UInventoryComponent::UInventoryComponent()
	:
	InventoryCapacity(15), InventoryItemTotalWeight(0.f),
	NearbyItemIndex(0)
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
	ScrollNearbyItemList(0);
	APickupItem* ItemToGetIn = NearbyItemList[NearbyItemIndex];
	ServerAddItemToPlayerInventory(ItemToGetIn);
}

void UInventoryComponent::ServerAddItemToPlayerInventory_Implementation(APickupItem* Item)
{
	MulticastAddItemToPlayerInventory(Item);
}

void UInventoryComponent::MulticastAddItemToPlayerInventory_Implementation(APickupItem* Item)
{
	AddItemToInventory(Item);
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
		
	if(const auto EquipableItem = Cast<IEquipable>(ItemToIn))
	{
		FEquipItemEvent EquipItemEvent;
		EquipItemEvent.AddDynamic(this, &UInventoryComponent::EquipItem);
		EquipableItem->SetEquipItemEvent(EquipItemEvent);
	}
}

void UInventoryComponent::DropItemFromInventory(APickupItem* ItemToOut)
{
	if(ItemToOut == nullptr)	return;
	
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
	ScrollNearbyItemList(0);
}

void UInventoryComponent::DeleteNearbyItem(AActor* Item)
{
	const auto PickupItemActor = Cast<APickupItem>(Item);
	if(PickupItemActor == nullptr)	return;
	NearbyItemList.Remove(PickupItemActor);
	NearbyItemDeleteEvent.Broadcast(PickupItemActor);
	ScrollNearbyItemList(0);
}

void UInventoryComponent::EquipItem(APickupItem* Item)
{
	PlayerCombatComponent->EquipItemToCharacter(Item);
}

void UInventoryComponent::GenerateItemsToWorld()
{
	if(InventoryItemList.IsEmpty())	return;
	for(const auto Item : InventoryItemList)
	{
		Item->DropItem();
	}
	InventoryItemList.Empty();
}

void UInventoryComponent::ScrollNearbyItemList(int32 AddAmount)
{
	if(NearbyItemList.IsEmpty())	return;
	NearbyItemIndex += AddAmount;
	NearbyItemIndex = (NearbyItemList.Num() <= 1) ?
						0 :
						FMath::Clamp(NearbyItemIndex, 0, NearbyItemList.Num() - 1);
	NearbyListScrollChangedEvent.Broadcast(NearbyItemIndex);
}