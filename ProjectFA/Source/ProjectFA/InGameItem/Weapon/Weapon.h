// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "Weapon.generated.h"

/**
 * 
 */

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

public:

	AWeapon();
	UFUNCTION()
	void AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	
	virtual void BeginPlay() override;
};
