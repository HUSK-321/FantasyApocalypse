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

UCLASS()
class PROJECTFA_API AProjectFAPlayGameMode : public AGameMode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APickupItem>> ItemTable;
	UPROPERTY(EditAnywhere)
	TArray<UItemSpawnPool*> ItemSpawnPools;
	
public:
	void PlayerDead(APlayableCharacter* VictimCharacter, APlayableController* VictimController, APlayableController* InstigatorController);
	void SpawnItemToAllSpawner();

protected:
	virtual void HandleMatchIsWaitingToStart() override;

private:
	TArray<APickupItem*> GetRandomItemList(IItemSpawnable* Spawner);
};
