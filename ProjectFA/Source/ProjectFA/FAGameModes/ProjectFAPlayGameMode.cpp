// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectFAPlayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "ProjectFA/PlayGamePretreatment/ItemSpawnable.h"
#include "ProjectFA/PlayGamePretreatment/ItemSpawnPool.h"

void AProjectFAPlayGameMode::PlayerDead(APlayableCharacter* VictimCharacter, APlayableController* VictimController,
                                        APlayableController* InstigatorController)
{
}

void AProjectFAPlayGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
	
	InitializeSpawnPoolList();
	SpawnItemToAllSpawner();
}

void AProjectFAPlayGameMode::InitializeSpawnPoolList()
{
	for(const auto PoolClass : ItemSpawnPoolClasses)
	{
		auto ItemPool = NewObject<UItemSpawnPool>(this, PoolClass);
		ItemSpawnPools.Emplace(ItemPool);
	}
}

void AProjectFAPlayGameMode::SpawnItemToAllSpawner()
{
	if(GetWorld() == nullptr)	return;
	TArray<AActor*> OutSpawnerList;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UItemSpawnable::StaticClass(), OutSpawnerList);
	for(const auto Spawner : OutSpawnerList)
	{
		const auto Spawnable = Cast<IItemSpawnable>(Spawner);
		if(Spawnable == nullptr)	return;
		
		Spawnable->SetSpawnItemList(GetRandomItemList(Spawnable));
	}
}

TArray<APickupItem*> AProjectFAPlayGameMode::GetRandomItemList(IItemSpawnable* Spawner)
{
	TArray<APickupItem*> ItemList;
	if(Spawner == nullptr || GetWorld() == nullptr)
	{
		return ItemList;
	}
	
	auto SpawnInfoList = Spawner->GetSpawnCategoryPercent();
	for(const auto SpawnInfo : SpawnInfoList)
	{
		const auto AmountToSpawn = FMath::RandRange(SpawnInfo.SpawnAmountMin, SpawnInfo.SpawnAmountMax);
		for(int32 SpawnCount = 0; SpawnCount < AmountToSpawn; SpawnCount++)
		{
			const auto Item = ItemSpawnPools[SpawnInfo.CategoryIndex]->GetItemFromPool();
			Item->SetItemPropertyFromDataAsset(GetRandomItemData(SpawnInfo.CategoryIndex));
			ItemList.Emplace(Item);
		}
	}
	return ItemList;
}

UItemDataAsset* AProjectFAPlayGameMode::GetRandomItemData(int32 CategoryIndex)
{
	const int32 DataCount = ItemDataTables[CategoryIndex]->GetRowNames().Num();
	const int32 ItemIndex = FMath::RandRange(0, DataCount - 1);
	const FName ItemNameInRow = ItemDataTables[CategoryIndex]->GetRowNames()[ItemIndex];
	const FItemDataTable* ItemRow = ItemDataTables[CategoryIndex]->FindRow<FItemDataTable>(ItemNameInRow, TEXT(""));
	if(ItemRow == nullptr)	return nullptr;

	return ItemRow->ItemDataAsset;	
}