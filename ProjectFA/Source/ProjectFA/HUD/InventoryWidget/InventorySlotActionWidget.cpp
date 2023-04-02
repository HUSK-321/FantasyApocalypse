// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotActionWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"

void UInventorySlotActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto Position = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	const auto Canvas = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemActionBox);
	Canvas->SetPosition(Position);
}

void UInventorySlotActionWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}