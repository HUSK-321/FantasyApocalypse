// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectFA/FACharacter/SkillSystem/SkillDataAsset.h"

void USkillSlotWidget::SetSkill(USkillDataAsset* SkillDataAsset)
{
	SkillData = SkillDataAsset;
	if(const auto Data = SkillData.Get())
	{
		SkillImage->SetBrushFromTexture(Data->GetThumbnail());
		Data->SkillCoolTimeStartEvent.AddDynamic(this, &USkillSlotWidget::SetCoolDownWidgetVisible);
		Data->SkillDoingEvent.AddDynamic(this, &USkillSlotWidget::SetSkillCoolTimeText);
		Data->SkillCoolTimeEndEvent.AddDynamic(this, &USkillSlotWidget::SetCoolDownWidgetHidden);
		SetCoolDownWidgetHidden();
	}
}

void USkillSlotWidget::SetSkillImage(UTexture2D* Image)
{
	SkillImage->SetBrushFromTexture(Image);

	SetCooldownWidgetVisibility(ESlateVisibility::Hidden);
}

void USkillSlotWidget::SetSkillCoolTimeText(const float& RemainTime)
{
	FNumberFormattingOptions Options;
	Options.SetMinimumFractionalDigits(2);
	Options.SetMaximumFractionalDigits(2);
	CooldownTimer->SetText(FText::AsNumber(RemainTime, &Options));
}

void USkillSlotWidget::SetCooldownWidgetVisibility(const ESlateVisibility SlateVisibility)
{
	CooldownImage->SetVisibility(SlateVisibility);
	CooldownTimer->SetVisibility(SlateVisibility);
}

void USkillSlotWidget::SetCoolDownWidgetVisible()
{
	SetCooldownWidgetVisibility(ESlateVisibility::Visible);
}

void USkillSlotWidget::SetCoolDownWidgetHidden()
{
	SetCooldownWidgetVisibility(ESlateVisibility::Hidden);
}