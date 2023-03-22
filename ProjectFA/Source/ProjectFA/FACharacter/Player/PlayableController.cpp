// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableController.h"
#include "InventoryComponent.h"
#include "PlayableCharacter.h"
#include "Components/ListView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ProjectFA/HUD/PlayerOverlay.h"
#include "ProjectFA/HUD/ProjectFAHUD.h"
#include "ProjectFA/HUD/PickupItemListWidget/PickupItemList.h"
#include "ProjectFA/InGameItem/PickupItem.h"

void APlayableController::BeginPlay()
{
	Super::BeginPlay();

	ProjectFAHUD = Cast<AProjectFAHUD>(GetHUD());
}

void APlayableController::SetPlayerEvent(APlayableCharacter* ControllingPlayer)
{
	ControllingPlayer->PlayerHealthChangedEvent.AddDynamic(this, &APlayableController::SetHealthHUD);
	ControllingPlayer->PlayerStaminaChangedEvent.AddDynamic(this, &APlayableController::SetStaminaHUD);
}

void APlayableController::SetInventoryEvent(UInventoryComponent* InventoryComponent)
{
	InventoryComponent->NearbyItemAddEvent.AddDynamic(this, &APlayableController::AddNearbyItem);
	InventoryComponent->NEarbyItemDeleteEvent.AddDynamic(this, &APlayableController::DeleteNearbyItem);
}

void APlayableController::SetHealthHUD(const float& CurrentHealth, const float& MaxHealth)
{
	const bool bHealthUIIsNull = ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->HealthText == nullptr || ProjectFAHUD->PlayerOverlay->HealthBar == nullptr;
	if(bHealthUIIsNull)	return;

	const auto& Overlay = ProjectFAHUD->PlayerOverlay;
	const float HealthPercent = CurrentHealth / MaxHealth;
	const FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(CurrentHealth), FMath::CeilToInt(MaxHealth));
	Overlay->HealthBar->SetPercent(HealthPercent);
	Overlay->HealthText->SetText(FText::FromString(HealthText));
}

void APlayableController::SetStaminaHUD(const float& CurrentStamina, const float& MaxStamina)
{
	const bool bHealthUIIsNull = ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->StaminaBar == nullptr;
	if(bHealthUIIsNull)	return;

	const float StaminaPercent = CurrentStamina / MaxStamina;
	ProjectFAHUD->PlayerOverlay->StaminaBar->SetPercent(StaminaPercent);
}

void APlayableController::AddNearbyItem(UObject* Item)
{
	const bool bItemListIsNull = ProjectFAHUD == nullptr || ProjectFAHUD->PickupItemList == nullptr || ProjectFAHUD->PickupItemList->NearbyItemList == nullptr;
	if(bItemListIsNull)	return;
	ProjectFAHUD->PickupItemList->NearbyItemList->AddItem(Item);
}

void APlayableController::DeleteNearbyItem(UObject* Item)
{
	const bool bItemListIsNull = ProjectFAHUD == nullptr || ProjectFAHUD->PickupItemList == nullptr || ProjectFAHUD->PickupItemList->NearbyItemList == nullptr;
	if(bItemListIsNull)	return;
	ProjectFAHUD->PickupItemList->NearbyItemList->RemoveItem(Item);
}