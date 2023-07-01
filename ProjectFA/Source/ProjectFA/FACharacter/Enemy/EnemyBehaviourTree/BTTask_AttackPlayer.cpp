// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
	NodeName = TEXT("AttackPlayer");
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControllingPawn == nullptr)	return EBTNodeResult::Failed;
	const auto EnemyController = ControllingPawn->GetController<IEnemyControllable>();
	if(EnemyController == nullptr)	return EBTNodeResult::Failed;

	EnemyController->SetControllingEnemyAttack();
	return EBTNodeResult::Succeeded;
}