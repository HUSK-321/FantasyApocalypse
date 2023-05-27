// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlotWidget.h"
#include "Components/Image.h"

void USkillSlotWidget::SetSkillImage(UTexture2D* Image)
{
	SkillImage->SetBrushFromTexture(Image);
}
