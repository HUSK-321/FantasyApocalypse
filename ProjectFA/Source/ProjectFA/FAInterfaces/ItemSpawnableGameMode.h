// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemSpawnableGameMode.generated.h"

UINTERFACE(MinimalAPI)
class UItemSpawnableGameMode : public UInterface
{
	GENERATED_BODY()
};

class APickupItem;

class PROJECTFA_API IItemSpawnableGameMode
{
	GENERATED_BODY()

public:
	virtual void InitializeSpawnPoolList(){}
	virtual void SpawnItemToAllSpawner(){}
	virtual void RequestSetItemArray(TArray<APickupItem*>& ItemList, UObject* Spawner) {}
};
