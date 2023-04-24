// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCollisionNotifyState.h"

#include "PlayableCharacterCombatComponent.h"

void UWeaponCollisionNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	const auto MeshOwnerActor = MeshComp->GetOwner();
	const auto ActorComponent = MeshOwnerActor->GetComponentByClass(UPlayableCharacterCombatComponent::StaticClass());
	if(const auto CombatComponent = Cast<UPlayableCharacterCombatComponent>(ActorComponent))
	{
		CombatComponent->WeaponAttacking();
	}
}

void UWeaponCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	const auto MeshOwnerActor = MeshComp->GetOwner();
	const auto ActorComponent = MeshOwnerActor->GetComponentByClass(UPlayableCharacterCombatComponent::StaticClass());
	if(const auto CombatComponent = Cast<UPlayableCharacterCombatComponent>(ActorComponent))
	{
		CombatComponent->WeaponAttackEnd();
	}
}
