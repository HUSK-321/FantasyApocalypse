// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectFA/FAInterfaces/WeaponAttackableComponent.h"
#include "PlayableCharacterCombatComponent.generated.h"


class APlayableCharacter;
class AFACharacter;
class AWeapon;
class UEquipable;
class APickupItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API UPlayableCharacterCombatComponent : public UActorComponent, public IWeaponAttackableComponent
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TObjectPtr<AFACharacter> Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APickupItem> EquippedItem;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APickupItem> DefaultPunchWeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APickupItem> DefaultPunchWeapon;
	
	bool bNowAttacking;
	bool bDoNextAttack;

public:

	UPlayableCharacterCombatComponent();
	
	virtual void WeaponAttacking() override;
	virtual void WeaponAttackEnd() override;

	void EquipItemToCharacter(APickupItem* ItemToEquip);
	void Attack();
	UFUNCTION(BlueprintCallable)
	void CheckShouldStopAttack();
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	FORCEINLINE bool GetNowAttacking() const { return bNowAttacking; }
	
protected:
	
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ItemDrop(APickupItem* UnEquipItem);
};
