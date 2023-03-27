// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProjectFA/InGameItem/InventoryUsable.h"
#include "ProjectFA/InGameItem/PickupItem.h"

void UInventorySlot::InitializeInventorySlot(APickupItem* Item)
{
	SlotItem = Item;
	ItemName->SetText(FText::FromString(Item->GetItemName()));
	ItemButton->OnClicked.Clear();
	ItemButton->OnClicked.AddDynamic(this, &UInventorySlot::DoSlotItemAction);
}

void UInventorySlot::DoSlotItemAction()
{
	if(const auto ItemToInventoryAction = Cast<IInventoryUsable>(SlotItem))
	{
		ItemToInventoryAction->InventoryAction_Implementation();
	}
}