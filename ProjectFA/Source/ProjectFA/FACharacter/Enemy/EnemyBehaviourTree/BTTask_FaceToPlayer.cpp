// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FaceToPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FaceToPlayer::UBTTask_FaceToPlayer()
{
	NodeName = TEXT("Face To Player");
}

EBTNodeResult::Type UBTTask_FaceToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)	return EBTNodeResult::Failed;

	const auto TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetPlayerKey);
	const auto TargetActor = Cast<AActor>(TargetObject);
	if (IsValid(TargetActor) == false)	return EBTNodeResult::Failed;

	auto NewRotation = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), TargetActor->GetActorLocation());
	NewRotation.Pitch = 0.f;
	ControllingPawn->SetActorRotation(NewRotation);

	return EBTNodeResult::Succeeded;
}
