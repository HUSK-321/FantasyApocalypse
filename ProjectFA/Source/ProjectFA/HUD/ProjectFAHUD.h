// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProjectFAHUD.generated.h"

/**
 * 
 */
class UPlayerOverlay;
class UPickupItemList;
class UInventoryWidget;
class USkillWidget;
class UDeadWidget;

UCLASS()
class PROJECTFA_API AProjectFAHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	void PlayerSetToDead();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;
	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<UPlayerOverlay> PlayerOverlay;
	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UPickupItemList> PickupItemListClass;
	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<UPickupItemList> PickupItemList;
	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<UInventoryWidget> Inventory;
	
	UPROPERTY(EditAnywhere, Category = "HUD|Dead")
	TSubclassOf<UDeadWidget> DeadWidgetClass;
	UPROPERTY(EditAnywhere, Category = "HUD|Dead")
	TObjectPtr<UDeadWidget> DeadWidget;
};
