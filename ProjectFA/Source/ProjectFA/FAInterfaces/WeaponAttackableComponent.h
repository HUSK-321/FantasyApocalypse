// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponAttackableComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponAttackableComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API IWeaponAttackableComponent
{
	GENERATED_BODY()

public:
	virtual void WeaponAttacking() = 0;
	virtual void WeaponAttackEnd() = 0;
};
