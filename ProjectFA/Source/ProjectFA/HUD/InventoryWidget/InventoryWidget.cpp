// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/WrapBox.h"

void UInventoryWidget::SetInventoryWidgetList(const TArray<APickupItem*>& ItemList)
{
	ItemWrapBox->ClearChildren();
	const auto PlayerController = GetOwningPlayer();
	if(PlayerController == nullptr || !InventorySlotClass)	return;
	
	for(auto Item : ItemList)
	{
		auto InventorySlotWidget = CreateWidget<UInventorySlotWidget>(PlayerController, InventorySlotClass);
		InventorySlotWidget->InitializeInventorySlot(Item);
		ItemWrapBox->AddChildToWrapBox(InventorySlotWidget);
	}
}
