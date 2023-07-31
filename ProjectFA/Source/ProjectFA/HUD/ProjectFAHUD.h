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
class APickupItem;
class USkillDataAsset;

UCLASS()
class PROJECTFA_API AProjectFAHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

	void PlayerSetToDead();

	void SetHealth(const float& CurrentHealth, const float& MaxHealth);
	void SetStamina(const float& CurrentStamina, const float& MaxStamina);

	void AddNearbyItem(UObject* Item);
	void DeleteNearbyItem(UObject* Item);
	void ScrollNearbyItemList(int32 ScrollIndex);
	void AddInventoryItem(const TArray<APickupItem*> ItemList);
	void SetInventoryWeight(const float& Weight);

	void InitializeSkillWidget(USkillDataAsset* QSkillData, USkillDataAsset* ESkillData);

	void CurrentHandItemWidget(APickupItem* ItemInHand);

	void SetAnnounceText(const FString& AnnounceText);
	void DisableAnnounce();

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
