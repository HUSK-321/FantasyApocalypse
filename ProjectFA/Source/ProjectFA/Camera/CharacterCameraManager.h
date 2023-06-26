// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CharacterCameraManager.generated.h"

UCLASS()
class PROJECTFA_API ACharacterCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> HitCameraShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|Sound", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> HitSound;

public:
	virtual void BeginPlay() override;
	
	void PlayHitCameraShake(); 
};