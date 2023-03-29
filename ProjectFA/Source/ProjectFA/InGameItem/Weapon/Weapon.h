// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/InGameItem/Equipable.h"
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

class UBoxComponent;

UCLASS()
class PROJECTFA_API AWeapon : public APickupItem, public IEquipable
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> AttackCollision;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	EWeaponType WeaponType;

public:

	AWeapon();
	
	virtual FName GetNormalAttackMontageSectionName() const override;
	virtual void SetAttackCollision(bool bEnable) override;

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
