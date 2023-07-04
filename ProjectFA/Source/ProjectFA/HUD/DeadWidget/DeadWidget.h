// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeadWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTFA_API UDeadWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SpectatedActorName;

public:
	void ViewTargetChanged(AActor* ViewTarget);
};