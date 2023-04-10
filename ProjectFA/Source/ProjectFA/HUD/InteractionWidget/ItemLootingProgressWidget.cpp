// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLootingProgressWidget.h"

void UItemLootingProgressWidget::SetProgressPercent(const float& Percent)
{
	ImageMaterialInstanceDynamic == nullptr ? ImageMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(ImageMaterialInstance, this) :
											  ImageMaterialInstanceDynamic;
	if(ImageMaterialInstanceDynamic == nullptr)	return;
	
	ImageMaterialInstanceDynamic->SetScalarParameterValue(FName("Percent"), Percent);
}
