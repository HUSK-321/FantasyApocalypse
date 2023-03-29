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

UCLASS()
class PROJECTFA_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeInventorySlot(APickupItem* Item);

private:

	UFUNCTION()
	void DoSlotItemAction();

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ItemButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemImage;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<APickupItem> SlotItem;
};
