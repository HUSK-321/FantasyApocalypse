// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectFAPlayGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "ProjectFA/PlayGamePretreatment/ItemSpawnable.h"
#include "ProjectFA/PlayGamePretreatment/ItemSpawnPool.h"

void AProjectFAPlayGameMode::PlayerDead(APlayableCharacter* VictimCharacter, APlayableController* VictimController,
                                        APlayableController* InstigatorController)
{
}

void AProjectFAPlayGameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	UE_LOG(LogTemp, Warning, TEXT("SwapPlayerControllers"))

	const auto OldPLayerState = OldPC->GetPlayerState<APlayerState>();
	const auto NewPLayerState = NewPC->GetPlayerState<APlayerState>();

	if(IsValid(OldPLayerState) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("OldPLayerState invalid"));
	}
	if(IsValid(NewPLayerState) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewPLayerState invalid"));
	}
	
	if(IsValid(OldPLayerState) && IsValid(NewPLayerState))
	{
		NewPLayerState->SetPlayerName(OldPLayerState->GetPlayerName());
		UE_LOG(LogTemp, Warning, TEXT("player name : %s"), *NewPLayerState->GetPlayerName());
	}
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
		const auto ItemPool = NewObject<UItemSpawnPool>(this, PoolClass);
		ItemPools.Emplace(ItemPool->GetItemCategory(), ItemPool);
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
		if(Spawnable == nullptr || Spawnable->IsItemSpawned())	return;
		
		Spawnable->SetSpawnItemList(GetRandomItemList(Spawnable));
	}
}

void AProjectFAPlayGameMode::RequestSetItemArray(TArray<APickupItem*>& ItemList, UObject* Spawner)
{
	const auto SpawnerClass = Cast<IItemSpawnable>(Spawner);
	ItemList = GetRandomItemList(SpawnerClass);
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
			ItemList.Emplace(GetItemFromPool(SpawnInfo.ItemCategory));
		}
	}
	return ItemList;
}

APickupItem* AProjectFAPlayGameMode::GetItemFromPool(FName ItemCategory)
{
	if(ItemPools.Contains(ItemCategory) == false)	return nullptr;
	const auto ItemFromPool = ItemPools[ItemCategory]->GetItemFromPool();
	if(ItemFromPool == nullptr)	return ItemFromPool;
	
	ItemFromPool->SetItemPropertyFromDataAsset(GetRandomItemDataAsset(ItemCategory));
	return ItemFromPool;
}

UItemDataAsset* AProjectFAPlayGameMode::GetRandomItemDataAsset(FName ItemCategory)
{
	const int32 DataCount = ItemDataTables[ItemCategory]->GetRowNames().Num();
	const int32 ItemIndex = FMath::RandRange(0, DataCount - 1);
	const FName ItemNameInRow = ItemDataTables[ItemCategory]->GetRowNames()[ItemIndex];
	const FItemDataTable* ItemRow = ItemDataTables[ItemCategory]->FindRow<FItemDataTable>(ItemNameInRow, TEXT(""));
	if(ItemRow == nullptr)	return nullptr;

	return ItemRow->ItemDataAsset;	
}
