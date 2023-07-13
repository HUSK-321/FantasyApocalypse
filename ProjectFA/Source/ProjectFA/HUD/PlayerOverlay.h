// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

/**
 * 
 */

class USkillWidget;
class UPlayerHandSlotWidget;
class UAnnouncementWidget;

UCLASS()
class PROJECTFA_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> StaminaBar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> HealthText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USkillWidget> SkillWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPlayerHandSlotWidget> HandSlotWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UAnnouncementWidget> AnnounceWidget;

public:
	void SetAnnounceText(FString TextToAnnounce);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableAnnounce();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableAnnounce();
};
