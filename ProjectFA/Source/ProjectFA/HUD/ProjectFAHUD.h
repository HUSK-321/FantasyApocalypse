// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProjectFAHUD.generated.h"

/**
 * 
 */
class UPlayerOverlay;

UCLASS()
class PROJECTFA_API AProjectFAHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void DrawHUD() override;

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;
	TObjectPtr<UPlayerOverlay> PlayerOverlay;
	
};
