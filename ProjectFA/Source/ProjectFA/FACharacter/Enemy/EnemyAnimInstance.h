// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
class AEnemy;

UCLASS()
class PROJECTFA_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AEnemy> Enemy;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void SetEnemyProperties();
};