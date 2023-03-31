// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupableCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupableCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API IPickupableCharacter
{
	GENERATED_BODY()

public:

	virtual void SetNearbyItem(AActor* Item) = 0;
	virtual void UnsetNearbyItem(AActor* Item) = 0;
};