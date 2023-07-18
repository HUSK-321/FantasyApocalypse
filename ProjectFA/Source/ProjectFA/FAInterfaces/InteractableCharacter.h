// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableCharacter.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API IInteractableCharacter
{
	GENERATED_BODY()

public:
	virtual void SetInteractingActor(AActor* Actor) = 0;
};
