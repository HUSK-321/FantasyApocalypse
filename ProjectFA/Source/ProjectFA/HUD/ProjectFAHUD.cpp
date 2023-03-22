// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectFAHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.h"
#include "PickupItemListWidget/PickupItemList.h"

void AProjectFAHUD::BeginPlay()
{
	Super::BeginPlay();
	
	const auto PlayerController = GetOwningPlayerController();
	if(PlayerController && PlayerOverlayClass)
	{
		PlayerOverlay = CreateWidget<UPlayerOverlay>(PlayerController, PlayerOverlayClass);
		PlayerOverlay->AddToViewport();
		PickupItemList = CreateWidget<UPickupItemList>(PlayerController, PickupItemListCLass);
		PickupItemList->AddToViewport();
	}
}

void AProjectFAHUD::DrawHUD()
{
	Super::DrawHUD();
}