// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<AEnemy>(GetOwningActor());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	SetEnemyProperties();
}

void UEnemyAnimInstance::SetEnemyProperties()
{
	if(Enemy == nullptr)	return;

	auto EnemyVelocity = Enemy->GetVelocity();
	EnemyVelocity.Z = 0.f;
	Speed = EnemyVelocity.Size();
}