// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
class UWrapBox;
class APickupItem;
class UInventorySlotWidget;
class UTextBlock;

UCLASS()
class PROJECTFA_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetInventoryWidgetList(const TArray<APickupItem*>& ItemList);

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TotalWeight;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWrapBox> ItemWrapBox;
	UPROPERTY(EditAnywhere, Category = "Inventory HUD")
	TSubclassOf<UInventorySlotWidget> InventorySlotClass;
};
