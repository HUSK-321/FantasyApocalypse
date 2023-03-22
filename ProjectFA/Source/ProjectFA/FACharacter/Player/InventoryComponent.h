// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UPlayableCharacterCombatComponent;
class APickupItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneItemChangedEvnet, UObject*, Item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FOneItemChangedEvnet NearbyItemAddEvent;
	FOneItemChangedEvnet NearbyItemDeleteEvent;

private:

	UPROPERTY()
	TObjectPtr<UPlayableCharacterCombatComponent> PlayerCombatComponent;
	UPROPERTY(EditAnywhere)
	int8 InventoryCapacity;
	UPROPERTY()
	TArray<APickupItem*> NearbyItemList;
	UPROPERTY()
	TArray<APickupItem*> InventoryItemList;

public:
	
	UInventoryComponent();
	void AddNearbyItem(APickupItem* Item);
	void DeleteNearbyItem(APickupItem* Item);
	void SetNearbyItemToInventory();

protected:
	
	virtual void BeginPlay() override;
};
