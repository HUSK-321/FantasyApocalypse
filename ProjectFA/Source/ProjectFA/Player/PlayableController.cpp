// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ProjectFA/HUD/PlayerOverlay.h"
#include "ProjectFA/HUD/ProjectFAHUD.h"

void APlayableController::BeginPlay()
{
	Super::BeginPlay();

	ProjectFAHUD = Cast<AProjectFAHUD>(GetHUD());
}

void APlayableController::SetHealthHUD(const float& CurrentHealth, const float& MaxHealth)
{
	ProjectFAHUD = (ProjectFAHUD == nullptr) ? Cast<AProjectFAHUD>(GetHUD()) : ProjectFAHUD;
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
	ProjectFAHUD = (ProjectFAHUD == nullptr) ? Cast<AProjectFAHUD>(GetHUD()) : ProjectFAHUD;
	const bool bHealthUIIsNull = ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->StaminaBar == nullptr;
	if(bHealthUIIsNull)	return;

	const float StaminaPercent = CurrentStamina / MaxStamina;
	ProjectFAHUD->PlayerOverlay->StaminaBar->SetPercent(StaminaPercent);
}
