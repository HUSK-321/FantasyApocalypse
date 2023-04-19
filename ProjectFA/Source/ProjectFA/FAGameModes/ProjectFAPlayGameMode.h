// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectFAPlayGameMode.generated.h"

/**
 * 
 */

class IItemSpawnable;
class APlayableCharacter;
class APlayableController;
class APickupItem;
class UItemSpawnPool;
class UDataTable;
class UItemDataAsset;

UCLASS()
class PROJECTFA_API AProjectFAPlayGameMode : public AGameMode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APickupItem>> ItemTable;
	UPROPERTY(EditAnywhere)
	TArray<UItemSpawnPool*> ItemSpawnPools;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UDataTable>> ItemDataTables;
	
public:
	void PlayerDead(APlayableCharacter* VictimCharacter, APlayableController* VictimController, APlayableController* InstigatorController);

protected:
	virtual void HandleMatchIsWaitingToStart() override;

private:
	void InitializeSpawnPoolList();
	void SpawnItemToAllSpawner();
	
	TArray<APickupItem*> GetRandomItemList(IItemSpawnable* Spawner);
	UItemDataAsset* GetRandomItemData(int32 CategoryIndex);
};
