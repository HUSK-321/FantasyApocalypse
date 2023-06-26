// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombActor.generated.h"

UCLASS()
class PROJECTFA_API ABombActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BombMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", meta = (AllowPrivateAccess = "true"))
	float TimeToExplode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", meta = (AllowPrivateAccess = "true"))
	float DamageInnerRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", meta = (AllowPrivateAccess = "true"))
	float DamageOuterRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> BombSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb|Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> BombParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb|Effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShakeEffect;

	FTimerHandle ExplodeTimerHandle;
	
public:	
	ABombActor();
	
	UFUNCTION(BlueprintCallable)
	void ExplodeDamage();
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:	
	void SetExplodeTimer();
	UFUNCTION()
	void ExplodeTimerEnd();

	void SpawnBombEffects();
};