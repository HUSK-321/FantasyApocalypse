// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHandSlotWidget.generated.h"

class UImage;

UCLASS()
class PROJECTFA_API UPlayerHandSlotWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> WeaponImage;

public:
	void SetWeaponImage(UTexture2D* Image);
	
};