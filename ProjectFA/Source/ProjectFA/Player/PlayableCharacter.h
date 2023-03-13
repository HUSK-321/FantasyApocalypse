// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacter.generated.h"

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class APlayableController;

UCLASS()
class PROJECTFA_API APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraSpringArm;
	UPROPERTY()
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY()
	TObjectPtr<APlayableController> PlayableController;

	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxSprintSpeed;
	bool bNowSprinting;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float CurrentHealth;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float CurrentStamina;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float StaminaIncreaseFactor;
	UPROPERTY(EditAnywhere, Category = "Player Property")
	float StaminaDecreaseFactor;

public:
	
	APlayableCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

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
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);
	void SetSprinting(bool bSprinting);
	void ManageStaminaAmount(float DeltaTime);

	void SetHealthHUD();
	void SetStaminaHUD();
};
