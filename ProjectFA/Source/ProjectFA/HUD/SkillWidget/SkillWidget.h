// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

class USkillSlotWidget;
class USkillDataAsset;

UCLASS()
class PROJECTFA_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USkillSlotWidget> SkillSlotQ;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USkillSlotWidget> SkillSlotE;

public:
	void SetSkillSlotWidget(USkillDataAsset* QSkill, USkillDataAsset* ESkill);
	void SetSkillQCoolTimeVisible();
	void SetSkillECoolTimeVisible();
	void SetSkillQCoolTimeHidden();
	void SetSkillECoolTimeHidden();
	void SetSkillQTime(const float RemainTime);
	void SetSkillETime(const float RemainTime);
};