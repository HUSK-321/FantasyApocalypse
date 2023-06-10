// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackNotifyState.h"
#include "ProjectFA/FACharacter/Enemy/Enemy.h"

void UEnemyAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(MeshComp == nullptr)	return;
	const auto EnemyCharacter = Cast<AEnemy>(MeshComp->GetOwner());
	if(EnemyCharacter == nullptr)	return;

	EnemyCharacter->SetAttackCollision(true);
}

void UEnemyAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if(MeshComp == nullptr)	return;
	const auto EnemyCharacter = Cast<AEnemy>(MeshComp->GetOwner());
	if(EnemyCharacter == nullptr)	return;

	EnemyCharacter->SetAttackCollision(false);
}