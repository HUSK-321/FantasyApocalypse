// Fill out your copyright notice in the Description page of Project Settings.


#include "FootstepAnimNotify.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ProjectFA/FAInterfaces/SoundPlayableInstance.h"

UFootstepAnimNotify::UFootstepAnimNotify()
	:
	VolumeMultiplier(.5f)
{
}

void UFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(MeshComp == nullptr || MeshComp->GetWorld() == nullptr)	return;

	FHitResult HitResult;
	const FVector FootLocation = MeshComp->GetSocketLocation(BoneName);
	const FVector End{ FootLocation + FVector{0.f, 0.f, -400.f } };
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MeshComp->GetOwner());
	QueryParams.bReturnPhysicalMaterial = true;

	auto World = MeshComp->GetWorld();
	World->LineTraceSingleByChannel(HitResult, FootLocation, End, ECC_WorldStatic, QueryParams);
	if(HitResult.bBlockingHit == false)	return;
	
	if(auto SoundInstance = World->GetGameInstance<ISoundPlayableInstance>())
	{
		SoundInstance->PlayFootstepSoundPhysics(FootLocation, HitResult.PhysMaterial->SurfaceType, VolumeMultiplier);
	}
}
