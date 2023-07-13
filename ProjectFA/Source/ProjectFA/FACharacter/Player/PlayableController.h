// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectFA/FAInterfaces/Controller/ItemRPCableController.h"
#include "PlayableController.generated.h"

class APlayableCharacter;
class UInventoryComponent;
class AProjectFAHUD;
class APickupItem;
class USkillDataAsset;
class UPlayableCharacterCombatComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpectatorViewTargetChanged, AActor*);

UCLASS()
class PROJECTFA_API APlayableController : public APlayerController, public IItemRPCableController
{
	GENERATED_BODY()

public:
	FOnSpectatorViewTargetChanged OnSpectatorViewTargetChanged;

private:
	TObjectPtr<AProjectFAHUD> ProjectFAHUD;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnUnPossess() override;

public:	
	virtual void InteractingWithObject(UObject* LootingBox) override;
	virtual void DropItem(APickupItem* Item) override;
	virtual void UseItem(UObject* Item) override;

	void SetPlayerEvent(APlayableCharacter* ControllingPlayer);
	void SetInventoryEvent(UInventoryComponent* InventoryComponent);
	void SetCombatComponentEvent(UPlayableCharacterCombatComponent* CombatComponent);
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

	UFUNCTION()
	void InitializeSkillWidget(USkillDataAsset* QSkillData, USkillDataAsset* ESkillData);

	UFUNCTION()
	void CurrentHandItemWidget(APickupItem* ItemInHand);

	UFUNCTION()
	void AnnouncePlayer(FString AnnounceText);
	UFUNCTION()
	void DisableAnnounce();

	virtual void ViewAPlayer(int32 dir) override;
	void SetPlayerSpectate();
	UFUNCTION(Reliable, Client)
	void ClientViewTarget();
	UFUNCTION(Reliable, Client)
	void ClientHUDChangedToDead();

private:
	UFUNCTION()
	void SetInventoryWeight(const float& Weight);
	
	bool PlayerHealthOverlayNotValid() const;
	bool NearbyItemListNotValid() const;
	bool PlayerStaminaOverlayNotValid() const;
	bool InventoryWidgetNotValid() const;
	bool SkillWidgetNotValid() const;

	void SetInputModeGameAndUI();
	void SetInputModeGameOnly();

	UFUNCTION(Server, Reliable)
	void ServerInteracting(UObject* LootingBox);
	UFUNCTION(Server, Reliable)
	void ServerDropItem(APickupItem* Item);
	UFUNCTION(Server, Reliable)
	void ServerUseItem(UObject* Item);
};