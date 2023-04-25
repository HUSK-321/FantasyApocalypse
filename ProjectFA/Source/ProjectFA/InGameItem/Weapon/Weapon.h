// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/InGameItem/Equipable.h"
#include "ProjectFA/InGameItem/InventoryUsable.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "Weapon.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Default			UMETA(DisplayName = "Default"),
	EWT_OneHandSword	UMETA(DisplayName = "One Hand Sword"),
	EWT_MagicStaff		UMETA(DisplayName = "Magic Staff"),

	EWT_MAX				UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PROJECTFA_API AWeapon : public APickupItem, public IEquipable, public IInventoryUsable
{
	GENERATED_BODY()

public:
	FEquipItemEvent EquipItemEvent;
	FEquipItemEvent UnEquipEvent;

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	EWeaponType WeaponType;
	
	UPROPERTY()
	TArray<AActor*> HittedActors;

public:
	AWeapon();
	
	virtual FName GetNormalAttackMontageSectionName() const override;
	virtual void UnEquip() override;
	virtual void WeaponAttacking_Implementation() override;
	virtual void AttackEnd_Implementation() override;
	virtual void SetEquipItemEvent(const FEquipItemEvent& Event) override;
	virtual void SetUnEquipEvent(const FEquipItemEvent& Event) override;

	virtual void InventoryAction_Implementation() override;
	virtual void RemoveFromInventoryAction_Implementation() override;

protected:
	virtual void BeginPlay() override;
};
