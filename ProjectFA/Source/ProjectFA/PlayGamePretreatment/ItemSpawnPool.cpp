// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnPool.h"
#include "ProjectFA/InGameItem/PickupItem.h"

UItemSpawnPool::UItemSpawnPool()
{
}

void UItemSpawnPool::SetPoolItemClass(const TSubclassOf<APickupItem> ItemClass, UWorld* World)
{
	if(ItemClass == nullptr)	return;
	SpawnPoolItemClass = ItemClass;
	int32 SpawnCount = 5;
	while(SpawnCount--)
	{
		SupplyItemToPool(World);
	}
}

APickupItem* UItemSpawnPool::GetItemFromPool(UWorld* World)
{
	if(ItemPool.IsEmpty())
	{
		SupplyItemToPool(World);
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

void UItemSpawnPool::SupplyItemToPool(UWorld* World)
{
	if(SpawnPoolItemClass == nullptr || World == nullptr)	return;
	
	const auto CreatedItem = World->SpawnActor<APickupItem>(SpawnPoolItemClass);
	ItemPool.Enqueue(CreatedItem);
}