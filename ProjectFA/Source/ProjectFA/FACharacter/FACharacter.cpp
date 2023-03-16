// Fill out your copyright notice in the Description page of Project Settings.


#include "FACharacter.h"

AFACharacter::AFACharacter()
	:
	MaxHealth(100.f), CurrentHealth(100.f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFACharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFACharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
}