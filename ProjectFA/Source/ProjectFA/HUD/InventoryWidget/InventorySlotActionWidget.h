// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotActionWidget.generated.h"

/**
 * 
 */
class UVerticalBox;
class UButton;

UCLASS()
class PROJECTFA_API UInventorySlotActionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> ItemActionBox;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ActionButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> DropButton;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
