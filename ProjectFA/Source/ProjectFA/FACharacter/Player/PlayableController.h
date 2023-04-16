// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayableController.generated.h"

/**
 * 
 */
class APlayableCharacter;
class UInventoryComponent;
class AProjectFAHUD;
class APickupItem;

UCLASS()
class PROJECTFA_API APlayableController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	void SetPlayerEvent(APlayableCharacter* ControllingPlayer);
	void SetInventoryEvent(UInventoryComponent* InventoryComponent);
	UFUNCTION()
	void AddNearbyItem(UObject* Item);
	UFUNCTION()
	void DeleteNearbyItem(UObject* Item);
	UFUNCTION()
	void ScrollNearbyItemList(int32 ScrollIndex);

	UFUNCTION()
	void SetHealthHUD(const float& CurrentHealth, const float& MaxHealth);
	UFUNCTION()
	void SetStaminaHUD(const float& CurrentStamina, const float& MaxStamina);

	UFUNCTION()
	void AddInventoryItem(const TArray<APickupItem*> ItemList);
	UFUNCTION()
	void ToggleInventoryWidget();

private:

	UFUNCTION()
	void SetInventoryWeight(const float& Weight);
	
	bool PlayerHealthOverlayNotValid() const;
	bool NearbyItemListNotValid() const;
	bool PlayerStaminaOverlayNotValid() const;
	bool InventoryWidgetNotValid() const;

	void SetInputModeGameAndUI();
	void SetInputModeGameOnly();

private:

	TObjectPtr<AProjectFAHUD> ProjectFAHUD;
};
