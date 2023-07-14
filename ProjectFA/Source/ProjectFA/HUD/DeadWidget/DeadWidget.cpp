// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UDeadWidget::ViewTargetChanged(AActor* ViewTarget)
{
	if(SpectatedActorName == nullptr)	return;

	if(const auto ViewPawn = Cast<APawn>(ViewTarget))
	{
		if(ViewPawn->GetPlayerState())
		{
			SpectatedActorName->SetText(FText::FromString(ViewPawn->GetPlayerState()->GetPlayerName()));
		}
	}
}