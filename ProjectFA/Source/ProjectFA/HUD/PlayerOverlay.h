// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFA_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> StaminaBar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> HealthText;
	
};
