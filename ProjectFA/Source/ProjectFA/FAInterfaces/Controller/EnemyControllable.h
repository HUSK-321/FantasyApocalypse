// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyControllable.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyControllable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API IEnemyControllable
{
	GENERATED_BODY()

public:
	virtual void SetControllingEnemyAttack(){}
	virtual void SetEnemyBlackboardValueAsObject(const FName& KeyName, UObject* ObjectValue){}
	virtual void SetEnemyBlackboardValueAsVector(const FName& KeyName, FVector VectorValue){}
	virtual void SetEnemyBlackboardValueAsBool(const FName& KeyName, bool BoolValue){}
};