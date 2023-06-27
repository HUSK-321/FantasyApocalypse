// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GroggyEnemy.generated.h"

UCLASS()
class PROJECTFA_API AGroggyEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	AGroggyEnemy();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> SpawnTriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SpawnMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groggy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> WeakGroggyMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groggy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> StrongGroggyMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groggy", meta = (AllowPrivateAccess = "true"))
	int32 MaxWeakGroggyGauge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groggy", meta = (AllowPrivateAccess = "true"))
	int32 MaxStrongGroggyGauge;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Groggy", meta = (AllowPrivateAccess = "true"))
	int32 WeakGroggyGauge;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Groggy", meta = (AllowPrivateAccess = "true"))
	int32 StrongGroggyGauge;

public:
	void WeakGroggy(const FVector& DamageCauser);
	void StrongGroggy();

	virtual void PlayNormalAttackMontage(FName NormalAttackSectionName) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;

private:
	void SpawnEnemyByTriggerBox();
	void DealGroggyDamage(int32 WeakGroggyDamage, int32 StrongGroggyDamage, const FVector& DamageCauser);

	bool WeakGroggyMontagePlay(const FVector& DamageCauser);
	bool StrongGroggyMontagePlay();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastWeakGroggyMontagePlay(FName MontageSection);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStrongGroggyMontagePlay();

	UFUNCTION()
	void OnSpawnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnEnemy();
};