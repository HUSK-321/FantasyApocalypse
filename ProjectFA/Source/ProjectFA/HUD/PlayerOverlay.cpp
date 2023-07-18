// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerOverlay.h"
#include "Announce/AnnouncementWidget.h"

void UPlayerOverlay::SetAnnounceText(FString TextToAnnounce)
{
	if(AnnounceWidget)
	{
		AnnounceWidget->SetAnnounceText(TextToAnnounce);
		EnableAnnounce();
	}
}

void UPlayerOverlay::DisableAnnounce_Implementation()
{
}

void UPlayerOverlay::EnableAnnounce_Implementation()
{
}