// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayableController.generated.h"

/**
 * 
 */
class APlayableCharacter;
class AProjectFAHUD;

UCLASS()
class PROJECTFA_API APlayableController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

private:

	TObjectPtr<AProjectFAHUD> ProjectFAHUD;

public:

	void SetPlayerEvent(APlayableCharacter* ControllingPlayer);

	UFUNCTION()
	void SetHealthHUD(const float& CurrentHealth, const float& MaxHealth);
	UFUNCTION()
	void SetStaminaHUD(const float& CurrentStamina, const float& MaxStamina);
};
