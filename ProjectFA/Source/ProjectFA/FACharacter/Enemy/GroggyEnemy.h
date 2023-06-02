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

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;

private:
	void DealGroggyDamage(int32 WeakGroggyDamage, int32 StrongGroggyDamage, const FVector& DamageCauser);

	virtual void OnSpawnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};