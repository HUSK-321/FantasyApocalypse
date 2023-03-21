// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "PlayableCharacter.generated.h"

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UPlayableCharacterCombatComponent;
class UInventoryComponent;
class APickupItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerCurrentMaxDelegate, const float&, CurrentValue, const float&, MaxValue);

UCLASS()
class PROJECTFA_API APlayableCharacter : public AFACharacter
{
	GENERATED_BODY()

public:
	
	FPlayerCurrentMaxDelegate PlayerHealthChangedEvent;
	FPlayerCurrentMaxDelegate PlayerStaminaChangedEvent;

private:

	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraSpringArm;
	UPROPERTY()
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayableCharacterCombatComponent> CombatComponent;
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;	

	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxSprintSpeed;
	bool bNowSprinting;
	
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float CurrentStamina;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float StaminaIncreaseFactor;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float StaminaDecreaseFactor;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float JumpStaminaConsume;

	// TODO : change to TArray
	UPROPERTY()
	TObjectPtr<APickupItem> CurrentlyNearItem;

public:
	
	APlayableCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void SetCurrentPickupItem(APickupItem* PickupItem);

	FORCEINLINE UPlayableCharacterCombatComponent* GetPlayerCombatComponent() const { return CombatComponent; }

protected:
	
	virtual void BeginPlay() override;

private:

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnRight(float Value);
	void LookUp(float Value);
	virtual void Jump() override;
	void CrouchButtonPressed();
	void SprintButtonPressed();
	void SprintButtonReleased();
	void InteractionButtonPressed();
	void AttackButtonPressed();
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;
	void SetSprinting(bool bSprinting);
	void ManageStaminaAmount(float DeltaTime);
};
