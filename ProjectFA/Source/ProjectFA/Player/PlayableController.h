// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayableController.generated.h"

/**
 * 
 */
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

	void SetHealthHUD(const float& CurrentHealth, const float& MaxHealth);
	void SetStaminaHUD(const float& CurrentStamina, const float& MaxStamina);
};
