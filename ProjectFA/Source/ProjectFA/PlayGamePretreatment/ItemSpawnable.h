// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemSpawnable.generated.h"

class APickupItem;

UINTERFACE(MinimalAPI)
class UItemSpawnable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API IItemSpawnable
{
	GENERATED_BODY()

public:
	virtual const TArray<APickupItem*> GetItemListBySpawnIndex(int32 SpawnIndex) = 0;
};
