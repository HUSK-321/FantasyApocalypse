// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectFAPlayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "ProjectFA/PlayGamePretreatment/ItemSpawnable.h"

void AProjectFAPlayGameMode::PlayerDead(APlayableCharacter* VictimCharacter, APlayableController* VictimController,
                                        APlayableController* InstigatorController)
{
}

void AProjectFAPlayGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetWorld() == nullptr)	return;
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UItemSpawnable::StaticClass(), OutActors);
	for(const auto Spawner : OutActors)
	{
		const auto Spawnable = Cast<IItemSpawnable>(Spawner);
		if(Spawnable == nullptr)	return;
		Spawnable->SetSpawnItemList(GetRandomItemList());
	}
}

TArray<APickupItem*> AProjectFAPlayGameMode::GetRandomItemList()
{
	TArray<APickupItem*> SpawnItemList;
	int8 RandomItemCount = FMath::RandRange(1, 4);
	while(RandomItemCount--)
	{
		const int8 RandomItemIndex = FMath::RandRange(0, ItemTable.Num() - 1);
		auto Item = GetWorld()->SpawnActor<APickupItem>(ItemTable[RandomItemIndex]);
		SpawnItemList.Emplace(Item);
	}
	return SpawnItemList;
}