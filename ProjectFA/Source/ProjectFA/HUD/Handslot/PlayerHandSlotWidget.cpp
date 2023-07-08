// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHandSlotWidget.h"
#include "Components/Image.h"

void UPlayerHandSlotWidget::SetWeaponImage(UTexture2D* Image)
{
	WeaponImage->SetBrushFromTexture(Image);
}
