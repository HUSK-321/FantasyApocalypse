// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayableCharacterCombatComponent.generated.h"


class APlayableCharacter;
class AWeapon;
class UEquipable;
class APickupItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API UPlayableCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TObjectPtr<APlayableCharacter> PlayableCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APickupItem> EquippedItem;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APickupItem> DefaultPunchWeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APickupItem> DefaultPunchWeapon;

public:

	UPlayableCharacterCombatComponent();

	void EquipItemToCharacter(APickupItem* ItemToEquip);
	void Attack() const;
	UFUNCTION(BlueprintCallable)
	void SetWeaponAttackCollision(bool bEnabled);
	
protected:
	
	virtual void BeginPlay() override;
};
