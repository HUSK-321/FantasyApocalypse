// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillSlotWidget::SetSkillImage(UTexture2D* Image)
{
	SkillImage->SetBrushFromTexture(Image);

	SetCooldownWidgetVisibility(ESlateVisibility::Hidden);
}

void USkillSlotWidget::SetCooldownWidgetVisibility(const ESlateVisibility SlateVisibility)
{
	CooldownImage->SetVisibility(SlateVisibility);
	CooldownTimer->SetVisibility(SlateVisibility);
}