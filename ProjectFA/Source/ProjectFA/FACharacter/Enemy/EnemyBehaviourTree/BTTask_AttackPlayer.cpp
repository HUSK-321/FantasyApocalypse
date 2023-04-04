// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectFA/FACharacter/FACharacter.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
	NodeName = TEXT("AttackPlayer");
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	const auto EnemyCharacter = Cast<AFACharacter>(ControllingPawn);
	const auto TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetPlayerKey);
	const auto TargetActor = Cast<AActor>(TargetObject);
	if(TargetActor == nullptr || EnemyCharacter == nullptr)	return EBTNodeResult::Failed;

	auto NewRotation = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), TargetActor->GetActorLocation());
	NewRotation.Pitch = 0.f;
	ControllingPawn->SetActorRotation(NewRotation);
	EnemyCharacter->PlayNormalAttackMontage();
	return EBTNodeResult::Succeeded;
}