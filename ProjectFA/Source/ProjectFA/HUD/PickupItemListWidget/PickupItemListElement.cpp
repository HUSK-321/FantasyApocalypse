// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItemListElement.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectFA/InGameItem/PickupItem.h"

void UPickupItemListElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	const APickupItem* Item = Cast<APickupItem>(ListItemObject);
	if(Item == nullptr)	return;

	ItemName->SetText(FText::FromString(Item->GetItemName()));
	ItemImage->SetBrushFromTexture(Item->GetItemIcon());
	SetCursorDeactive();
}

void UPickupItemListElement::SetCursorActive()
{
	CursorImage->SetVisibility(ESlateVisibility::Visible);
}

void UPickupItemListElement::SetCursorDeactive()
{
	CursorImage->SetVisibility(ESlateVisibility::Hidden);
}