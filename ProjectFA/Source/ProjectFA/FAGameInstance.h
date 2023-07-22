// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FAInterfaces/SoundPlayableInstance.h"
#include "FAGameInstance.generated.h"

class UPlayerQuestObserver;

UCLASS()
class PROJECTFA_API UFAGameInstance : public UGameInstance, public ISoundPlayableInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> GrassFootstep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> MetalFootstep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> WoodFootstep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> WaterFootstep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> StoneFootstep;

public:
	UFAGameInstance();
	virtual void PlayFootstepSoundPhysics(FVector SoundLocation, TEnumAsByte<EPhysicalSurface> SurfaceType, float VolumeMultiplier) override;
};