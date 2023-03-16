// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayableCharacterCombatComponent.generated.h"


class APlayableCharacter;
class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API UPlayableCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TObjectPtr<APlayableCharacter> PlayableCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeapon> EquippedWeapon;

public:

	UPlayableCharacterCombatComponent();

	void EquipWeapon(AWeapon* WeaponToEquip);
	void Attack();
	
protected:
	
	virtual void BeginPlay() override;
};
