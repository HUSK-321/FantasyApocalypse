// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTooltipWidget.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;
class UMultiLineEditableText;

UCLASS()
class PROJECTFA_API UItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> ItemImage;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UMultiLineEditableText> ItemDescription;
};