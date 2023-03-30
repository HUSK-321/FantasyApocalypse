// Fill out your copyright notice in the Description page of Project Settings.


#include "FACharacter.h"

#include "Components/CapsuleComponent.h"

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

void AFACharacter::PlayNormalAttackMontage(FName NormalAttackSectionName)
{
	if(NormalAttackMontage == nullptr)	return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance == nullptr)	return;
	AnimInstance->Montage_Play(NormalAttackMontage);
	AnimInstance->Montage_JumpToSection(NormalAttackSectionName);
}

void AFACharacter::StopNormalAttackMontage()
{
	if(NormalAttackMontage == nullptr)	return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance == nullptr)	return;
	AnimInstance->Montage_Stop(0.f, NormalAttackMontage);
}

void AFACharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                 AController* InstigatorController, AActor* DamageCauser)
{
}

void AFACharacter::CharacterDead()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(DieMontage == nullptr || AnimInstance == nullptr)	return;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AnimInstance->Montage_Play(DieMontage);
	Controller->UnPossess();
}

void AFACharacter::AfterDeath()
{
}

bool AFACharacter::CharacterCannotMove()
{
	return false;
}

bool AFACharacter::CharacterCannotAttack()
{
	return false;
}
