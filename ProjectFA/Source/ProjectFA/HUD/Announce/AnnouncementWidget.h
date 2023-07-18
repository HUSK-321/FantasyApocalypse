// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnnouncementWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTFA_API UAnnouncementWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> AnnounceText;

public:
	void SetAnnounceText(FString TextToAnnounce);
};