// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SoundPlayableInstance.generated.h"

UINTERFACE(MinimalAPI)
class USoundPlayableInstance : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API ISoundPlayableInstance
{
	GENERATED_BODY()

public:
	virtual void PlayFootstepSoundPhysics(FVector SoundLocation, TEnumAsByte<EPhysicalSurface> SurfaceType, float VolumeMultiplier){};
};
