// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equipable.generated.h"

class APickupItem;

UINTERFACE(MinimalAPI)
class UEquipable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipItemEvent, APickupItem*, Item);

class PROJECTFA_API IEquipable
{
	GENERATED_BODY()

public:

	virtual FName GetNormalAttackMontageSectionName() const = 0;
	virtual void UnEquip() = 0;
	virtual void SetAttackCollision(bool bEnable) = 0;
	virtual void SetEquipItemEvent(const FEquipItemEvent& Event) = 0;
	virtual void SetUnEquipEvent(const FEquipItemEvent& Event) = 0;
};
