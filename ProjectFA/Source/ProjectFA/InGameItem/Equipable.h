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
DECLARE_DYNAMIC_DELEGATE_RetVal(float, FGetPlayerDamagePropertyDelegate);

class PROJECTFA_API IEquipable
{
	GENERATED_BODY()

public:
	virtual FName GetNormalAttackMontageSectionName() const = 0;
	virtual void UnEquip() = 0;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack Action")
	void AttackStart();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack Action")
	void AttackEnd();
	virtual void SetEquipItemEvent(const FEquipItemEvent& Event) = 0;
	virtual void SetUnEquipEvent(const FEquipItemEvent& Event) = 0;
	virtual void SetPlayerDamagePropertyDelegate(const FGetPlayerDamagePropertyDelegate& Event) {}

	virtual int32 GetWeakGroggyGauge() const { return 0; }
	virtual int32 GetStrongGroggyGauge() const { return 0; }
};
