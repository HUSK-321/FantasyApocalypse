// Fill out your copyright notice in the Description page of Project Settings.
#include "ProjectFAHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.h"
#include "InventoryWidget/InventoryWidget.h"
#include "PickupItemListWidget/PickupItemList.h"

void AProjectFAHUD::BeginPlay()
{
	Super::BeginPlay();
	
	const auto PlayerController = GetOwningPlayerController();
	if(PlayerController && PlayerOverlayClass && PickupItemListClass && InventoryWidgetClass)
	{
		PlayerOverlay = CreateWidget<UPlayerOverlay>(PlayerController, PlayerOverlayClass);
		PlayerOverlay->AddToViewport();
		PickupItemList = CreateWidget<UPickupItemList>(PlayerController, PickupItemListClass);
		PickupItemList->AddToViewport();
		PickupItemList->SetVisibility(ESlateVisibility::Hidden);
		Inventory = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);
		Inventory->AddToViewport();
		Inventory->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AProjectFAHUD::DrawHUD()
{
	Super::DrawHUD();
}
