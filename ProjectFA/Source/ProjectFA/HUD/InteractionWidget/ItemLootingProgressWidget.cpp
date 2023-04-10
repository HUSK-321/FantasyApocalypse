// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLootingProgressWidget.h"
#include "Components/Image.h"

void UItemLootingProgressWidget::SetProgressPercent(const float& Percent)
{
	if(ProgressImage->GetDynamicMaterial() == nullptr)	return;
	
	ProgressImage->GetDynamicMaterial()->SetScalarParameterValue(FName("Percent"), Percent);
}