// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Components/TextBlock.h"
#include "ProjectFA/InGameItem/PickupItem.h"

void UInventorySlot::InitializeInventorySlot(APickupItem* Item)
{
	ItemName->SetText(FText::FromString(Item->GetItemName()));
}
