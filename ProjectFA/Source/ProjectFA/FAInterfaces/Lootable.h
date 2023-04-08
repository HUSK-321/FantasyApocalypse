// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Lootable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULootable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API ILootable
{
	GENERATED_BODY()

public:
	virtual void GenerateItemsToWorld() = 0;
};
