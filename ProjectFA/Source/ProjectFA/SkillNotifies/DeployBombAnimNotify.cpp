// Fill out your copyright notice in the Description page of Project Settings.


#include "DeployBombAnimNotify.h"

void UDeployBombAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp == nullptr) return;
	const auto World = MeshComp->GetWorld();
	if(World == nullptr || DeployObjectClass == nullptr)	return;
	
	const auto SpawnPosition = MeshComp->GetComponentLocation();
	World->SpawnActor<AActor>(DeployObjectClass, SpawnPosition, FRotator::ZeroRotator);
}