// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */

class UButton;
class UTextBlock;
class UImage;
class APickupItem;
class UInventorySlotActionWidget;

UCLASS()
class PROJECTFA_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeInventorySlot(APickupItem* Item);

protected:

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:

	UFUNCTION()
	void DoSlotItemAction();

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ItemButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemImage;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<APickupItem> SlotItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventorySlotActionWidget> SlotActionWidgetClass;
};
