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

	FTimerHandle ExplodeTimerHandle;
	
public:	
	ABombActor();
	
	UFUNCTION(BlueprintCallable)
	void ExplodeDamage(); 
protected:
	virtual void BeginPlay() override;

private:	
	void SetExplodeTimer();
	UFUNCTION()
	void ExplodeTimerEnd();
};