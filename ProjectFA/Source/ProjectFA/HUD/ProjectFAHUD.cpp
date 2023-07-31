// Fill out your copyright notice in the Description page of Project Settings.
#include "ProjectFAHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.h"
#include "Components/ListView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "InventoryWidget/InventoryWidget.h"
#include "PickupItemListWidget/PickupItemList.h"
#include "DeadWidget/DeadWidget.h"
#include "Handslot/PlayerHandSlotWidget.h"
#include "ProjectFA/FACharacter/Player/PlayableController.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "SkillWidget/SkillWidget.h"

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

void AProjectFAHUD::PlayerSetToDead()
{
	if(PlayerOverlay)
	{
		PlayerOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
	if(PickupItemList)
	{
		PickupItemList->SetVisibility(ESlateVisibility::Hidden);
	}
	if(Inventory)
	{
		Inventory->SetVisibility(ESlateVisibility::Hidden);
	}

	const auto PlayerController = GetOwningPlayerController();
	if(DeadWidgetClass)
	{
		DeadWidget = CreateWidget<UDeadWidget>(PlayerController, DeadWidgetClass);
		DeadWidget->AddToViewport();
		if(const auto PlayableController = Cast<APlayableController>(PlayerController))
		{
			PlayableController->OnSpectatorViewTargetChanged.AddUObject(DeadWidget, &UDeadWidget::ViewTargetChanged);
		}
	}
}

void AProjectFAHUD::SetHealth(const float& CurrentHealth, const float& MaxHealth)
{
	if((IsValid(PlayerOverlay) && IsValid(PlayerOverlay->HealthBar) && IsValid(PlayerOverlay->HealthText)) == false)	return;
	
	const float HealthPercent = CurrentHealth / MaxHealth;
	const FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(CurrentHealth), FMath::CeilToInt(MaxHealth));
	PlayerOverlay->HealthBar->SetPercent(HealthPercent);
	PlayerOverlay->HealthText->SetText(FText::FromString(HealthText));
}

void AProjectFAHUD::SetStamina(const float& CurrentStamina, const float& MaxStamina)
{
	if(IsValid(PlayerOverlay->StaminaBar) == false)	return;
	
	const float StaminaPercent = CurrentStamina / MaxStamina;
	PlayerOverlay->StaminaBar->SetPercent(StaminaPercent);
}

void AProjectFAHUD::AddNearbyItem(UObject* Item)
{
	if((IsValid(PickupItemList) && IsValid(PickupItemList->NearbyItemList)) == false)	return;
	
	PickupItemList->NearbyItemList->AddItem(Item);
	PickupItemList->SetVisibility(ESlateVisibility::Visible);
}

void AProjectFAHUD::DeleteNearbyItem(UObject* Item)
{
	if((IsValid(PickupItemList->NearbyItemList) && IsValid(PickupItemList)) == false)	return;
	
	PickupItemList->NearbyItemList->RemoveItem(Item);
	if(PickupItemList->NearbyItemList->GetNumItems() == 0)
	{
		PickupItemList->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AProjectFAHUD::ScrollNearbyItemList(int32 ScrollIndex)
{
	if(IsValid(PickupItemList) == false)	return;
	
	PickupItemList->SetScrollIndex(ScrollIndex);
}

void AProjectFAHUD::AddInventoryItem(const TArray<APickupItem*> ItemList)
{
	if(IsValid(Inventory) == false)	return;
	
	Inventory->SetInventoryWidgetList(ItemList);
}

void AProjectFAHUD::SetInventoryWeight(const float& Weight)
{
	if((IsValid(Inventory) && IsValid(Inventory->TotalWeight)) == false)	return;
	
	const FString WeightTextString = FString::Printf(TEXT("Total Weight : %.2fKG"), Weight); 
	Inventory->TotalWeight->SetText(FText::FromString(WeightTextString));
}

void AProjectFAHUD::InitializeSkillWidget(USkillDataAsset* QSkillData, USkillDataAsset* ESkillData)
{
	if((IsValid(PlayerOverlay) && IsValid(PlayerOverlay->SkillWidget)) == false)	return;
	
	PlayerOverlay->SkillWidget->SetSkillSlotWidget(QSkillData, ESkillData);
}

void AProjectFAHUD::CurrentHandItemWidget(APickupItem* ItemInHand)
{
	if((IsValid(PlayerOverlay) && IsValid(PlayerOverlay->HandSlotWidget)) == false)	return;
	
	PlayerOverlay->HandSlotWidget->SetWeaponImage(ItemInHand->GetItemIcon());
}

void AProjectFAHUD::SetAnnounceText(const FString& AnnounceText)
{
	if(IsValid(PlayerOverlay) == false)	return;
	
	PlayerOverlay->SetAnnounceText(AnnounceText);
}

void AProjectFAHUD::DisableAnnounce()
{
	if(IsValid(PlayerOverlay) == false)	return;
	
	PlayerOverlay->DisableAnnounce();
}