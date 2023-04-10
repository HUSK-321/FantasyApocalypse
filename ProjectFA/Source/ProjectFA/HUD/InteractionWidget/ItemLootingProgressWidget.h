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
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ImageMaterialInstanceDynamic;
	UPROPERTY(EditAnywhere, Category = "Progress", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> ImageMaterialInstance;
};
