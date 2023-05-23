// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISpawnableSkill.generated.h"

UINTERFACE(MinimalAPI)
class UISpawnableSkill : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API IISpawnableSkill
{
	GENERATED_BODY()

public:
	virtual void SpawnSkillObject();	
};