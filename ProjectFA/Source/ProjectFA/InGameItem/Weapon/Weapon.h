// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "Weapon.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial		UMETA(DisplayName = "Initial State"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_Dropped		UMETA(DisplayName = "Dropped"),
	EWS_InInventory	UMETA(DisplayName = "InInventory"),

	EWS_MAX			UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Default			UMETA(DisplayName = "Default"),
	EWT_OneHandSword	UMETA(DisplayName = "One Hand Sword"),
	EWT_MagicStaff		UMETA(DisplayName = "Magic Staff"),

	EWT_MAX				UMETA(DisplayName = "DefaultMAX")
};


class UBoxComponent;

UCLASS()
class PROJECTFA_API AWeapon : public APickupItem
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> AttackCollision;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TSubclassOf<UDamageType> DamageTypeClass;
	EWeaponState WeaponState;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	EWeaponType WeaponType;

public:

	AWeapon();
	
	static FName GetWeaponSectionName(const AWeapon* Weapon);
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponAttackCollision(bool bEnable);
	UFUNCTION()
	void SetWeaponState(const EWeaponState State);
	UFUNCTION()
	void AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	
	virtual void BeginPlay() override;
};
