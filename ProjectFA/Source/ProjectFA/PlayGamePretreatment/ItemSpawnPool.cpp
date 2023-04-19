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

APickupItem* UItemSpawnPool::GetItemFromPool()
{
	if(ItemPool.IsEmpty())
	{
		SupplyItemToPool(GetWorld());
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
	if(SpawnPoolItemClass == nullptr)	return;
	if(World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("get world nullptr"));
		return;
	}
	const auto CreatedItem = World->SpawnActor<APickupItem>(SpawnPoolItemClass);
	// TODO : 아이템 초기화 작업 Setitemstate에 추가하자
	ItemPool.Enqueue(CreatedItem);
}