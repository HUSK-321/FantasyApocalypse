// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCollisionNotifyState.h"
#include "PlayableCharacterCombatComponent.h"

void UWeaponCollisionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	const auto MeshOwnerActor = MeshComp->GetOwner();
	if(MeshOwnerActor == nullptr)	return; 
	const auto ActorComponents = MeshOwnerActor->GetComponentsByInterface(UWeaponAttackableComponent::StaticClass());
	for(const auto ActorComponent : ActorComponents)
	{
		const auto WeaponAttackable = Cast<IWeaponAttackableComponent>(ActorComponent);
		if(WeaponAttackable == nullptr)	continue;
		WeaponAttackable->WeaponAttackStart();
	}
}

void UWeaponCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	const auto MeshOwnerActor = MeshComp->GetOwner();
	if(MeshOwnerActor == nullptr)	return; 
	const auto ActorComponents = MeshOwnerActor->GetComponentsByInterface(UWeaponAttackableComponent::StaticClass());
	for(const auto ActorComponent : ActorComponents)
	{
		const auto WeaponAttackable = Cast<IWeaponAttackableComponent>(ActorComponent);
		if(WeaponAttackable == nullptr)	continue;
		WeaponAttackable->WeaponAttackEnd();
	}
}