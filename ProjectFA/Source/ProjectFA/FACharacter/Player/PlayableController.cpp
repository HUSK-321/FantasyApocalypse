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
	InventoryComponent->NearbyItemDeleteEvent.AddDynamic(this, &APlayableController::DeleteNearbyItem);
}

void APlayableController::SetHealthHUD(const float& CurrentHealth, const float& MaxHealth)
{
	if(PlayerHealthOverlayNotValid())	return;
	const auto& Overlay = ProjectFAHUD->PlayerOverlay;
	const float HealthPercent = CurrentHealth / MaxHealth;
	const FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(CurrentHealth), FMath::CeilToInt(MaxHealth));
	Overlay->HealthBar->SetPercent(HealthPercent);
	Overlay->HealthText->SetText(FText::FromString(HealthText));
}

void APlayableController::SetStaminaHUD(const float& CurrentStamina, const float& MaxStamina)
{
	if(PlayerStaminaOverlayNotValid())	return;
	const float StaminaPercent = CurrentStamina / MaxStamina;
	ProjectFAHUD->PlayerOverlay->StaminaBar->SetPercent(StaminaPercent);
}

void APlayableController::AddNearbyItem(UObject* Item)
{
	if(NearbyItemListNotValid())	return;
	ProjectFAHUD->PickupItemList->NearbyItemList->AddItem(Item);
	ProjectFAHUD->PickupItemList->SetVisibility(ESlateVisibility::Visible);
}


void APlayableController::DeleteNearbyItem(UObject* Item)
{
	if(NearbyItemListNotValid())	return;
	ProjectFAHUD->PickupItemList->NearbyItemList->RemoveItem(Item);
	if(ProjectFAHUD->PickupItemList->NearbyItemList->GetNumItems() == 0)
	{
		ProjectFAHUD->PickupItemList->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool APlayableController::PlayerHealthOverlayNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->HealthText == nullptr || ProjectFAHUD->PlayerOverlay->HealthBar == nullptr;
}

bool APlayableController::NearbyItemListNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->PickupItemList == nullptr || ProjectFAHUD->PickupItemList->NearbyItemList == nullptr;
}

bool APlayableController::PlayerStaminaOverlayNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->StaminaBar == nullptr;
}