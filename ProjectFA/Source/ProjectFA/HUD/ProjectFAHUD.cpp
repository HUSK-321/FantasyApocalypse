// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectFAHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.h"

void AProjectFAHUD::BeginPlay()
{
	Super::BeginPlay();
	
	const auto PlayerController = GetOwningPlayerController();
	if(PlayerController && PlayerOverlayClass)
	{
		PlayerOverlay = CreateWidget<UPlayerOverlay>(PlayerController, PlayerOverlayClass);
		PlayerOverlay->AddToViewport();
	}
}

void AProjectFAHUD::DrawHUD()
{
	Super::DrawHUD();
}