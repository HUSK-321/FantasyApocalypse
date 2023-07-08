// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadWidget.h"
#include "Components/TextBlock.h"

void UDeadWidget::ViewTargetChanged(AActor* ViewTarget)
{
	if(SpectatedActorName == nullptr)	return;

	const auto Name = FText::FromString(ViewTarget->GetActorNameOrLabel());
	SpectatedActorName->SetText(Name);
}
