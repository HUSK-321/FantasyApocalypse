// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LootableActor.generated.h"

UINTERFACE(MinimalAPI)
class ULootableActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API ILootableActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Looting Action")
	void FindItem(const float SearchTime);
};
