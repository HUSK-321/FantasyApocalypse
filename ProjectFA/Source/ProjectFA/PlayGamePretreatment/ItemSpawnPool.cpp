// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnPool.h"
#include "ProjectFA/InGameItem/PickupItem.h"

UItemSpawnPool::UItemSpawnPool()
{
}

void UItemSpawnPool::SetPoolItemClass(const TSubclassOf<APickupItem> ItemClass)
{
	if(ItemClass == nullptr)	return;
	SpawnPoolItemClass = ItemClass;
	int32 SpawnCount = 5;
	while(SpawnCount--)
	{
		SupplyItemToPool();
	}
}

APickupItem* UItemSpawnPool::GetItemFromPool()
{
	if(ItemPool.IsEmpty())
	{
		SupplyItemToPool();
	}

	APickupItem* ItemToReturn;
	if(ItemPool.Dequeue(ItemToReturn))
	{
		return ItemToReturn;
	}
	return nullptr;
}

void UItemSpawnPool::ReturnItemToPool(APickupItem* Item)
{
	if(Item == nullptr)	return;
	ItemPool.Enqueue(Item);
}

void UItemSpawnPool::SupplyItemToPool()
{
	if(SpawnPoolItemClass == nullptr || GetWorld() == nullptr)	return;

	if(const auto CreatedItem = GetWorld()->SpawnActor<APickupItem>(SpawnPoolItemClass))
	{
		ItemPool.Enqueue(CreatedItem);
		CreatedItem->SetItemState(EItemState::EIS_MAX);
	}
}