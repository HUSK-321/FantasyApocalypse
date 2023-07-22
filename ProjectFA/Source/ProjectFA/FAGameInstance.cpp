// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameInstance.h"
#include "ProjectFA.h"
#include "Kismet/GameplayStatics.h"

UFAGameInstance::UFAGameInstance()
{
}

void UFAGameInstance::PlayFootstepSoundPhysics(FVector SoundLocation, TEnumAsByte<EPhysicalSurface> SurfaceType, float VolumeMultiplier)
{
	USoundBase* FootStepSound;
	
	switch (SurfaceType)
	{
	case EPS_Grass :
		FootStepSound = GrassFootstep;
		break;
		
	case EPS_Metal :
		FootStepSound = MetalFootstep;
		break;

	case EPS_Wood :
		FootStepSound = WoodFootstep;
		break;

	case EPS_Water :
		FootStepSound = WaterFootstep;
		break;

	case EPS_Stone :	
		FootStepSound = StoneFootstep;
		break;

	default:
		FootStepSound = WoodFootstep;
		break;
	}

	if(FootStepSound == nullptr)	return;
	UGameplayStatics::PlaySoundAtLocation(this, FootStepSound, SoundLocation, FRotator::ZeroRotator, VolumeMultiplier);
}