// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectFA/FACharacter/Player/PlayableCharacter.h"

void ACharacterCameraManager::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(GetOwningPlayerController()) == false)	return;

	if(const auto Player = GetOwningPlayerController()->GetPawn<APlayableCharacter>())
	{
		Player->OnPlayerHitDelegate.AddUObject(this, &ACharacterCameraManager::PlayHitCameraShake);
	}
}

void ACharacterCameraManager::PlayHitCameraShake()
{
	GetOwningPlayerController()->ClientStartCameraShake(HitCameraShake);

	if((IsValid(HitSound) && IsValid(GetOwningPlayerController()) && IsValid(GetOwningPlayerController()->GetPawn())) == false)	return;
	
	const auto PlayerLocation = GetOwningPlayerController()->GetPawn()->GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(GetOwningPlayerController(), HitSound, PlayerLocation, FRotator::ZeroRotator);	
}