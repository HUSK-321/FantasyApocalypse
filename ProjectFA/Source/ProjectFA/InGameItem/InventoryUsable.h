// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryUsable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryUsable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API IInventoryUsable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryAction")
	void InventoryAction();
};
