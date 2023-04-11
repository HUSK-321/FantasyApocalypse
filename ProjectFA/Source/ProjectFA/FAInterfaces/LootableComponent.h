// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LootableComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULootableComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API ILootableComponent
{
	GENERATED_BODY()

public:
	virtual void GenerateItemsToWorld() = 0;
};
