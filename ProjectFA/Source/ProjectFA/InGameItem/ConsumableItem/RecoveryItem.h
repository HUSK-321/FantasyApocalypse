// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/InGameItem/InventoryUsable.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "RecoveryItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFA_API ARecoveryItem : public APickupItem, public IInventoryUsable
{
	GENERATED_BODY()

public:
	virtual void InventoryAction_Implementation() override;
	virtual void RemoveFromInventoryAction_Implementation() override;
};
