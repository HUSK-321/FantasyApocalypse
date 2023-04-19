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
	
	for(const auto ItemClass : ItemTable)
	{
		auto ItemPool = NewObject<UItemSpawnPool>();
		ItemPool->SetPoolItemClass(ItemClass, GetWorld());
		ItemSpawnPools.Emplace(ItemPool);
	}
	UE_LOG(LogTemp, Warning, TEXT("item spawn pool count : %d"), ItemSpawnPools.Num());
	
	SpawnItemToAllSpawner();
}

void AProjectFAPlayGameMode::SpawnItemToAllSpawner()
{
	if(GetWorld() == nullptr)	return;
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UItemSpawnable::StaticClass(), OutActors);
	for(const auto Spawner : OutActors)
	{
		const auto Spawnable = Cast<IItemSpawnable>(Spawner);
		if(Spawnable == nullptr)	return;
		
		Spawnable->SetSpawnItemList(GetRandomItemList(Spawnable));
	}
}

TArray<APickupItem*> AProjectFAPlayGameMode::GetRandomItemList(IItemSpawnable* Spawner)
{
	TArray<APickupItem*> ItemList;
	if(Spawner == nullptr)
	{
		return ItemList;
	}
	
	auto SpawnInfoList = Spawner->GetSpawnCategoryPercent();
	for(const auto SpawnInfo : SpawnInfoList)
	{
		const auto AmountToSpawn = FMath::RandRange(SpawnInfo.SpawnAmountMin, SpawnInfo.SpawnAmountMax);
		for(int SpawnCount = 0; SpawnCount < AmountToSpawn; SpawnCount++)
		{
			const auto Item = ItemSpawnPools[SpawnInfo.CategoryIndex]->GetItemFromPool();
			ItemList.Emplace(Item);
		}
	}
	return ItemList;
}