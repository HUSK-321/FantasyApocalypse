// Fill out your copyright notice in the Description page of Project Settings.


#include "SetMeshVisualAnimNotify.h"

USetMeshVisualAnimNotify::USetMeshVisualAnimNotify()
	:
	bMeshActive(false)
{
}

void USetMeshVisualAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp == nullptr)	return;
	MeshComp->SetVisibility(bMeshActive);
}
