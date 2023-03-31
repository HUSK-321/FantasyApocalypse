// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "Components/Button.h"
#include "ProjectFA/InGameItem/InventoryUsable.h"
#include "ProjectFA/InGameItem/PickupItem.h"

void UInventorySlotWidget::InitializeInventorySlot(APickupItem* Item)
{
	if(Item == nullptr)	return;
	SlotItem = Item;
	ItemButton->OnClicked.Clear();
	ItemButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::DoSlotItemAction);
}

void UInventorySlotWidget::DoSlotItemAction()
{
	if(const auto ItemToInventoryAction = Cast<IInventoryUsable>(SlotItem))
	{
		ItemToInventoryAction->InventoryAction_Implementation();
	}
}