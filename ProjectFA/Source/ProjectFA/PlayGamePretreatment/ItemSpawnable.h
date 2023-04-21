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

USTRUCT(Atomic, BlueprintType)
struct FSpawnerInitializeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CategoryIndex;
	// TODO : 이후 함수처럼 만드는 방법 생각해보기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnAmountMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnAmountMax;
};

class PROJECTFA_API IItemSpawnable
{
	GENERATED_BODY()

public:
	virtual TArray<FSpawnerInitializeInfo> GetSpawnCategoryPercent() = 0;
	virtual void SetSpawnItemList(const TArray<APickupItem*>& ItemList) = 0;
};
