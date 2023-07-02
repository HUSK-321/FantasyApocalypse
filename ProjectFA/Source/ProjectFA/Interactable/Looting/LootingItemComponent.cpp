// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingItemComponent.h"
#include "GameFramework/GameMode.h"
#include "ProjectFA/FAInterfaces/ItemSpawnableGameMode.h"
#include "ProjectFA/InGameItem/PickupItem.h"

ULootingItemComponent::ULootingItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULootingItemComponent::BeginPlay()
{
	Super::BeginPlay();

	if(IsItemSpawned() == false)
	{
		RequestItem();
	}
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

void ULootingItemComponent::RequestItem()
{
	const auto World = GetWorld();
	if(World == nullptr)	return;
	const auto SpawnerGameMode = Cast<IItemSpawnableGameMode>(World->GetAuthGameMode<AGameMode>());
	if(SpawnerGameMode == nullptr)	return;

	SpawnerGameMode->RequestSetItemArray(ItemList, GetOwner());
	for(auto Item : ItemList)
	{
		Item->SetOwner(GetOwner());
	}
}

bool ULootingItemComponent::IsItemSpawned()
{
	return ItemList.Num() > 0;
}