// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayableCharacterCombatComponent.generated.h"


class APlayableCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API UPlayableCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TObjectPtr<APlayableCharacter> PlayableCharacter;
	UPROPERTY()
	TObjectPtr<AActor> EquippedWeapon;

public:

	UPlayableCharacterCombatComponent();

	void EquipWeapon(AActor* WeaponToEquip);
	void Attack();
	
protected:
	
	virtual void BeginPlay() override;
};
