// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemLootingProgressWidget.generated.h"

/**
 * 
 */

class UImage;

UCLASS()
class PROJECTFA_API UItemLootingProgressWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetProgressPercent(const float& Percent);

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ProgressImage;
};
