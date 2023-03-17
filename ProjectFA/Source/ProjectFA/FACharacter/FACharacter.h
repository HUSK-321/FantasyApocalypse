// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FACharacter.generated.h"

class UInputComponent;

UCLASS()
class PROJECTFA_API AFACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AFACharacter();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	
	UFUNCTION()
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);

protected:

	UPROPERTY(EditAnywhere, Category = "Character Property")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Character Property")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Character Combat")
	TObjectPtr<UAnimMontage> NormalAttackMontage;

};
