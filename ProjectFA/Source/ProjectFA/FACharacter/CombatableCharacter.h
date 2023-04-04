// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatableCharacter.generated.h"

UINTERFACE(MinimalAPI)
class UCombatableCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API ICombatableCharacter
{
	GENERATED_BODY()

public:
	virtual UActorComponent* GetCombatComponent() const = 0;
};
