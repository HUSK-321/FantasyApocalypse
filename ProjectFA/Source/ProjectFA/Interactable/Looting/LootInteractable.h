// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LootInteractable.generated.h"

UINTERFACE(MinimalAPI)
class ULootInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API ILootInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Looting Action")
	void FindItem(const float SearchTime);

	virtual void OpenLooting(){}
};
