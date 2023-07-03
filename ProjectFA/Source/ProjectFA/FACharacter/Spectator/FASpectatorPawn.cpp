// Fill out your copyright notice in the Description page of Project Settings.


#include "FASpectatorPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AFASpectatorPawn::AFASpectatorPawn()
{
	bAddDefaultMovementBindings = false;
}

void AFASpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(GetController()->GetStateName() != NAME_Spectating)	return;
	
	if(const auto PlayerController = Cast<APlayerController>(GetController()))
	{
		if(const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SpectorBasicMappingContext, 0);
		}
	}
	
	if(const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->ClearActionBindings();
		EnhancedInputComponent->ClearActionValueBindings();
		EnhancedInputComponent->BindAction(NextPlayerAction, ETriggerEvent::Started, this, &AFASpectatorPawn::SpectateNextPlayer);
		EnhancedInputComponent->BindAction(PrevPlayerAction, ETriggerEvent::Started, this, &AFASpectatorPawn::SpectatePrevPlayer);
		EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Started, this, &AFASpectatorPawn::EscapeActionPressed);
	}

	SpectateNextPlayer();
}

void AFASpectatorPawn::SpectateNextPlayer()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->ServerViewNextPlayer();
	}
}

void AFASpectatorPawn::SpectatePrevPlayer()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->ServerViewPrevPlayer();
	}
}

void AFASpectatorPawn::EscapeActionPressed()
{
}