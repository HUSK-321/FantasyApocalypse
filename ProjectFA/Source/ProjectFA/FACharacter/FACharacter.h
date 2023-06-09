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

protected:
	UPROPERTY(EditAnywhere, Category = "Character Property")
	FName CharacterName;
	UPROPERTY(EditAnywhere, Category = "Character Property")
	float MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealthChanged ,EditAnywhere, Category = "Character Property")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Character Combat")
	TObjectPtr<UAnimMontage> NormalAttackMontage;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> HitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> HitSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Dead", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;
	UPROPERTY(EditAnywhere, Category = "Dead", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(VisibleAnywhere, Category = "Dead", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> DissolveTimeline;
	UPROPERTY(EditAnywhere, Category = "Dead")
	TObjectPtr<UCurveFloat> DissolveCurve;

	int8 bNowInDeadProcess : 1;

public:
	AFACharacter();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
	virtual void PlayNormalAttackMontage(FName NormalAttackSectionName = FName("Default"));
	UFUNCTION(BlueprintCallable)
	void StopNormalAttackMontage();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastPlayHitEffect(FVector EffectLocation);

	FORCEINLINE FName GetCharacterName() const { return CharacterName; }
	FORCEINLINE bool NowInDeadProcess() const { return bNowInDeadProcess; }

protected:
	UFUNCTION()
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);
	void CharacterDead();
	virtual void GenerateInventoryItems(){};
	
	UFUNCTION(BlueprintCallable)
	virtual bool CharacterCannotMove();
	UFUNCTION(BlueprintCallable)
	virtual bool CharacterCannotAttack();
	UFUNCTION(BlueprintCallable)
	virtual bool CharacterCannotJump();

	void StartDeadDissolve();

	/* This function call in OnRep_CurrentHealthChanged */
	virtual void CurrentHealthChanged();
	UFUNCTION()
	void OnRep_CurrentHealthChanged();

private:
	UFUNCTION()
	void UpdateMaterialDissolve(float DissolveTime);
	UFUNCTION()
	void AfterDeadDissolve();
};
