// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectFA/FAInterfaces/LootableComponent.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "InventoryComponent.generated.h"

class UPlayableCharacterCombatComponent;
class APickupItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneItemChangedEvent, UObject*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNearbyListScrollChangedEvent, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChangedEvent, const TArray<APickupItem*>, ItemList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryWeightChangedEvent, const float&, InventoryWeight);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API UInventoryComponent : public UActorComponent, public ILootableComponent
{
	GENERATED_BODY()

public:
	FOneItemChangedEvent NearbyItemAddEvent;
	FOneItemChangedEvent NearbyItemDeleteEvent;
	FNearbyListScrollChangedEvent NearbyListScrollChangedEvent;
	FInventoryChangedEvent InventoryChangedEvent;
	FInventoryWeightChangedEvent InventoryWeightChangedEvent;

private:
	UPROPERTY()
	TObjectPtr<UPlayableCharacterCombatComponent> PlayerCombatComponent;
	UPROPERTY(EditAnywhere)
	int8 InventoryCapacity;
	UPROPERTY(VisibleAnywhere)
	float InventoryItemTotalWeight;
	UPROPERTY()
	TArray<APickupItem*> NearbyItemList;
	UPROPERTY(VisibleAnywhere)
	TArray<APickupItem*> InventoryItemList;

	int32 NearbyItemIndex;

public:
	UInventoryComponent();
	
	void ScrollNearbyItemList(int32 AddAmount);
	UFUNCTION()
	void AddNearbyItem(AActor* Item);
	UFUNCTION()
	void DeleteNearbyItem(AActor* Item);
	void SetNearbyItemToInventory();

	virtual void GenerateItemsToWorld() override;

	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryItemTotalWeight; }

protected:
	virtual void BeginPlay() override;

private:
	void AddInventoryWeight(const float& ItemWeightToIn);
	void SubtractInventoryWeight(const float& ItemWeightToOut);
	void AddItemToInventory(APickupItem* ItemToIn);
	UFUNCTION()
	void DropItemFromInventory(APickupItem* ItemToOut);
	UFUNCTION()
	void EquipItem(APickupItem* Item);
};
