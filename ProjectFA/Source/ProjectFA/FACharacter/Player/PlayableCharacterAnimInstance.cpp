// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacterAnimInstance.h"
#include "PlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayableCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayableCharacter = Cast<APlayableCharacter>(TryGetPawnOwner());
}

void UPlayableCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	SetPlayableCharacterProperties();
}

void UPlayableCharacterAnimInstance::SetPlayableCharacterProperties()
{
	if(PlayableCharacter == nullptr)	return;
	auto PlayerVelocity = PlayableCharacter->GetVelocity();
	Velocity = PlayerVelocity;
	PlayerVelocity.Z = 0.f;

	Speed = PlayerVelocity.Size();
	bIsInAir = PlayableCharacter->GetCharacterMovement()->IsFalling();
	bIsCrouching = PlayableCharacter->bIsCrouched;
}
