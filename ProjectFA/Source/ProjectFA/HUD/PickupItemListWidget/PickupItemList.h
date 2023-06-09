// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupItemList.generated.h"

/**
 * 
 */

class UListView;
class UTextBlock;

UCLASS()
class PROJECTFA_API UPickupItemList : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UListView> NearbyItemList;

public:
	void SetScrollIndex(int32 ScrollIndex);
};
