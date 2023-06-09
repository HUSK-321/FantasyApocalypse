// Fill out your copyright notice in the Description page of Project Settings.


#include "DeployActorAnimNotify.h"

void UDeployActorAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp == nullptr || DeployObjectClass == nullptr) return;
	const auto World = MeshComp->GetWorld();
	const auto OwnerRole = MeshComp->GetOwnerRole();
	if(World == nullptr || OwnerRole < ROLE_Authority)	return;
	
	const auto SpawnPosition = MeshComp->GetComponentLocation() + (SpawnOffset * MeshComp->GetComponentRotation().Vector());
	World->SpawnActor<AActor>(DeployObjectClass, SpawnPosition, FRotator::ZeroRotator);
}