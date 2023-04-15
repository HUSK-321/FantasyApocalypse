// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItemList.h"
#include "PickupItemListElement.h"
#include "Components/ListView.h"

void UPickupItemList::SetScrollIndex(int32 ScrollIndex)
{
	NearbyItemList->ScrollIndexIntoView(ScrollIndex);

	const auto ItemCount = NearbyItemList->GetListItems().Num();
	for(int SlotIndex = 0; SlotIndex < ItemCount; SlotIndex++)
	{
		const auto SlotObject = NearbyItemList->GetItemAt(SlotIndex);
		if(const auto SlotWidget= Cast<UPickupItemListElement>(NearbyItemList->GetEntryWidgetFromItem(SlotObject)))
		{
			SlotWidget->SetCursorDeactive();
		}
	}
	const auto IndexItem = NearbyItemList->GetItemAt(ScrollIndex);
	if(IndexItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("count"));
	}
	if(const auto ActiveSlot = Cast<UPickupItemListElement>(NearbyItemList->GetEntryWidgetFromItem(IndexItem)))
	{
		UE_LOG(LogTemp, Warning, TEXT("active"));
		ActiveSlot->SetCursorActive();
	}
}