// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
	NodeName = TEXT("AttackPlayer");
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	const auto EnemyController = ControllingPawn->GetController<IEnemyControllable>();
	const auto TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetPlayerKey);
	const auto TargetActor = Cast<AActor>(TargetObject);
	if(IsValid(TargetActor) == false || EnemyController == nullptr)	return EBTNodeResult::Failed;

	auto NewRotation = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), TargetActor->GetActorLocation());
	NewRotation.Pitch = 0.f;
	ControllingPawn->SetActorRotation(NewRotation);

	EnemyController->SetControllingEnemyAttack();
	return EBTNodeResult::Succeeded;
}