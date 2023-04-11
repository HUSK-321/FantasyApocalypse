// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FACharacter.generated.h"

class UInputComponent;
class UTimelineComponent;

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
	UFUNCTION(BlueprintCallable)
	virtual void PlayNormalAttackMontage(FName NormalAttackSectionName = FName("Default"));
	UFUNCTION(BlueprintCallable)
	void StopNormalAttackMontage();

protected:
	
	UFUNCTION()
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);
	virtual void CharacterDead();
	UFUNCTION(BlueprintCallable)
	virtual void AfterDeath();
	UFUNCTION(BlueprintCallable)
	virtual bool CharacterCannotMove();
	UFUNCTION(BlueprintCallable)
	virtual bool CharacterCannotAttack();

	void StartDeadDissolve();

private:
	UFUNCTION()
	void UpdateMaterialDissolve(float DissolveTime);
	UFUNCTION()
	void AfterDeadDissolve();

protected:

	UPROPERTY(EditAnywhere, Category = "Character Property")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Character Property")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Character Combat")
	TObjectPtr<UAnimMontage> NormalAttackMontage;
	UPROPERTY(EditAnywhere, Category = "Character Combat")
	TObjectPtr<UAnimMontage> DieMontage;

private:
	UPROPERTY(VisibleAnywhere, Category = "Dead", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;
	UPROPERTY(EditAnywhere, Category = "Dead", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(VisibleAnywhere, Category = "Dead", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> DissolveTimeline;
	UPROPERTY(EditAnywhere, Category = "Dead")
	TObjectPtr<UCurveFloat> DissolveCurve;

};
