// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "InputActionValue.h"
#include "ProjectFA/FACharacter/CombatableCharacter.h"
#include "ProjectFA/FACharacter/InteractableCharacter.h"
#include "ProjectFA/FACharacter/PickupableCharacter.h"
#include "PlayableCharacter.generated.h"

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UPlayableCharacterCombatComponent;
class UInventoryComponent;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerCurrentMaxDelegate, const float&, CurrentValue, const float&, MaxValue);
DECLARE_MULTICAST_DELEGATE(FOnPlayerHitDelegate);

UCLASS()
class PROJECTFA_API APlayableCharacter : public AFACharacter, public IPickupableCharacter, public ICombatableCharacter, public IInteractableCharacter
{
	GENERATED_BODY()

public:
	FPlayerCurrentMaxDelegate PlayerHealthChangedEvent;
	FPlayerCurrentMaxDelegate PlayerStaminaChangedEvent;

	FOnPlayerHitDelegate OnPlayerHitDelegate;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayableCharacterCombatComponent> CombatComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> PlayerBasicMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InteractMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractNearbyItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractWithObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillEAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillQAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ScrollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HandSlot1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HandSlot2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HandSlot3Action;

	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxSprintSpeed;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxCrouchSpeed;
	UPROPERTY(VisibleAnywhere)
	bool bNowSprinting;
	
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float CurrentStamina;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float StaminaIncreaseFactor;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float StaminaDecreaseFactor;
	
	float InventoryWeightFactor;

	float InteractingTime;
	UPROPERTY()
	TObjectPtr<AActor> InteractingActor;

public:
	APlayableCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void SetNearbyItem(AActor* PickupItem) override;
	virtual void UnsetNearbyItem(AActor* PickupItem) override;

	virtual UActorComponent* GetCombatComponent() const override;

	virtual void SetInteractingActor(AActor* Actor) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void GenerateInventoryItems() override;
	
	virtual bool CharacterCannotMove() override;
	virtual bool CharacterCannotAttack() override;
	virtual bool CharacterCannotJump() override;

	virtual void CurrentHealthChanged() override;
	
private:
	UFUNCTION(Server, Reliable)
	void ServerSetCharacterMovement(bool bSprinting);

	void CharacterMove(const FInputActionValue& Value);
	void CameraMove(const FInputActionValue& Value);
	virtual void Jump() override;
	void CrouchButtonPressed();
	void SprintButtonPressed();
	void SprintButtonReleased();
	void InteractWithNearbyItem();
	void InteractWithActors(const FInputActionValue& Value);
	void InteractWithActorsEnd();
	void AttackButtonPressed();
	void SkillEButtonPressed();
	void SkillQButtonPressed();
	void ScrollNearbyItemList(const FInputActionValue& Value);
	void InventoryButtonPressed();
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;
	void SetSprinting(bool bSprinting);
	void ManageStaminaAmount(float DeltaTime);

	void HandSlot1Pressed();
	void HandSlot2Pressed();
	void HandSlot3Pressed();

	void SetInteractMappingContext(bool bIsActive);

	void SetCharacterMoveSpeed();
	UFUNCTION()
	void SetInventoryWeightSpeedFactor(const float& InventoryTotalWeight);
};
