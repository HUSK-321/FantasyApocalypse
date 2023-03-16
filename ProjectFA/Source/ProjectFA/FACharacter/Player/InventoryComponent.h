// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class APlayableCharacter;
class UPlayableCharacterCombatComponent;
class APickupItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TObjectPtr<APlayableCharacter> PlayableCharacter;
	UPROPERTY()
	TObjectPtr<UPlayableCharacterCombatComponent> PlayerCombatComponent;
	UPROPERTY()
	TArray<APickupItem*> ItemList;

public:
	
	UInventoryComponent();
	void SetPickupItemToInventory(APickupItem* InteractableItem);

protected:
	
	virtual void BeginPlay() override;
};
