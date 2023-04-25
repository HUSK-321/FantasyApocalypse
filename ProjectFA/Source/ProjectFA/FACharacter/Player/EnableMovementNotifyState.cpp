// Fill out your copyright notice in the Description page of Project Settings.


#include "EnableMovementNotifyState.h"

void UEnableMovementNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	MeshComp->GetAnimInstance()->RootMotionMode = ERootMotionMode::IgnoreRootMotion;
	
}

void UEnableMovementNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	MeshComp->GetAnimInstance()->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
}