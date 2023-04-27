// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Queue.h"
#include "ItemSpawnPool.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class PROJECTFA_API UItemSpawnPool : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APickupItem> SpawnPoolItemClass;

	TQueue<APickupItem*> ItemPool;

public:
	UItemSpawnPool();

	void SetPoolItemClass(const TSubclassOf<APickupItem> ItemClass);
	
	APickupItem* GetItemFromPool();
	void ReturnItemToPool(APickupItem* Item);

private:
	void SupplyItemToPool();
};