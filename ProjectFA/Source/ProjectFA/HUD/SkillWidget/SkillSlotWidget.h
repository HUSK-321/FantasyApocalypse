// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlotWidget.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;
class USkillDataAsset;

UCLASS()
class PROJECTFA_API USkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> SkillImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> CooldownImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> CooldownTimer;

	UPROPERTY()
	TWeakObjectPtr<USkillDataAsset> SkillData;

public:
	void SetSkill(USkillDataAsset* SkillDataAsset);
	void SetSkillImage(UTexture2D* Image);
	UFUNCTION()
	void SetSkillCoolTimeText(const float& RemainTime);
	void SetCooldownWidgetVisibility(ESlateVisibility SlateVisibility);

private:
	UFUNCTION()
	void SetCoolDownWidgetVisible();
	UFUNCTION()
	void SetCoolDownWidgetHidden();
};