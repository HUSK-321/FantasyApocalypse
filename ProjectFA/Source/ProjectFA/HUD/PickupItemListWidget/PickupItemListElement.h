// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "PickupItemListElement.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;

UCLASS()
class PROJECTFA_API UPickupItemListElement : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemImage;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	
};
