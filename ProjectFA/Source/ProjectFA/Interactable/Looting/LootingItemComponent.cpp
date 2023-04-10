// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingItemComponent.h"
#include "ProjectFA/InGameItem/PickupItem.h"

ULootingItemComponent::ULootingItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULootingItemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULootingItemComponent::InitializeItemList(const TArray<APickupItem*>& List)
{
	ItemList = List;
	for(APickupItem* Item : ItemList)
	{
		Item->SetOwner(GetOwner());
		Item->SetItemState(EItemState::EIS_InInventory);
	}
}

void ULootingItemComponent::GenerateItemsToWorld()
{
	if(ItemList.IsEmpty())	return;
	for(APickupItem* Item : ItemList)
	{
		Item->DropItem();
	}
	ItemList.Empty();
}