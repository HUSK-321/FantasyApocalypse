// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDescription.generated.h"

/**
 * 
 */
class UMultiLineEditableText;

UCLASS()
class PROJECTFA_API UItemDescription : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UMultiLineEditableText> ItemDescription;
	
};
