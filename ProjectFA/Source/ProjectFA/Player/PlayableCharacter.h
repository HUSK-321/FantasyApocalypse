// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacter.generated.h"

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECTFA_API APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	
	TObjectPtr<USpringArmComponent> CameraSpringArm;
	TObjectPtr<UCameraComponent> FollowCamera;

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
	void SetSprinting(bool bSprinting);
	void ManageStaminaAmount(float DeltaTime);

};
