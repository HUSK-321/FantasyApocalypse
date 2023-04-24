// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCollisionNotifyState.h"
#include "PlayableCharacterCombatComponent.h"

void UWeaponCollisionNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	const auto MeshOwnerActor = MeshComp->GetOwner();
	const auto ActorComponents = MeshOwnerActor->GetComponentsByInterface(UWeaponAttackableComponent::StaticClass());
	for(const auto ActorComponent : ActorComponents)
	{
		const auto WeaponAttackable = Cast<IWeaponAttackableComponent>(ActorComponent);
		if(WeaponAttackable == nullptr)	continue;
		WeaponAttackable->WeaponAttacking();
	}
}

void UWeaponCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	const auto MeshOwnerActor = MeshComp->GetOwner();
	const auto ActorComponents = MeshOwnerActor->GetComponentsByInterface(UWeaponAttackableComponent::StaticClass());
	for(const auto ActorComponent : ActorComponents)
	{
		const auto WeaponAttackable = Cast<IWeaponAttackableComponent>(ActorComponent);
		if(WeaponAttackable == nullptr)	continue;
		WeaponAttackable->WeaponAttackEnd();
	}
}