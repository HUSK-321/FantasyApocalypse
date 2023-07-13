// Fill out your copyright notice in the Description page of Project Settings.


#include "AnnouncementWidget.h"
#include "Components/TextBlock.h"

void UAnnouncementWidget::SetAnnounceText(FString TextToAnnounce)
{
	if(AnnounceText == nullptr)	return;
	AnnounceText->SetText(FText::FromString(TextToAnnounce));
}