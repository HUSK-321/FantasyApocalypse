// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlotWidget.h"
#include "Components/Button.h"
#include "ProjectFA/InGameItem/InventoryUsable.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "ItemTooltipWidget.h"
#include "InventorySlotActionWidget.h"
#include "Components/MultiLineEditableText.h"

void UInventorySlotWidget::InitializeInventorySlot(APickupItem* Item)
{
	if(Item == nullptr)	return;
	SlotItem = Item;
	ItemButton->OnClicked.Clear();
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryToolTipWidget = CreateWidget<UItemTooltipWidget>(this, InventoryToolTipClass);
	ItemButton->SetToolTip(GetToolTipWidget());
}

FReply UInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(SlotActionWidgetClass == nullptr || InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	}
	
	const auto ActionWidget = CreateWidget<UInventorySlotActionWidget>(this, SlotActionWidgetClass);
	if(ActionWidget == nullptr)	return  Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	ActionWidget->AddToViewport();
	ActionWidget->ActionButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::DoSlotItemAction);
	ActionWidget->DropButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::DropItemAction);
	
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::DoSlotItemAction()
{
	if(const auto ItemToInventoryAction = Cast<IInventoryUsable>(SlotItem))
	{
		ItemToInventoryAction->InventoryAction_Implementation();
	}
}

void UInventorySlotWidget::DropItemAction()
{
	if(const auto ItemToInventoryAction = Cast<IInventoryUsable>(SlotItem))
	{
		ItemToInventoryAction->RemoveFromInventoryAction_Implementation();
	}	
}

UUserWidget* UInventorySlotWidget::GetToolTipWidget()
{
	if(InventoryToolTipWidget == nullptr)	return nullptr;
	if(const auto Item = SlotItem.Get())
	{
		const auto ItemDescriptionText = FText::FromString(Item->GetItemDescription()); 
		InventoryToolTipWidget->ItemDescription->SetText(ItemDescriptionText);
	}
	return InventoryToolTipWidget;
}