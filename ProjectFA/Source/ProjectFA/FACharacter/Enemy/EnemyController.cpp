// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AEnemyController::AEnemyController()
	:
	EnemyBlackboardComponent(CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"))),
	EnemyBehaviorTreeComponent(CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTreeComponent")))
{
	
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	const AEnemy* Enemy = Cast<AEnemy>(InPawn);
	if(Enemy == nullptr || Enemy->GetEnemyBehaviorTree() == nullptr)	return;
	
	EnemyBlackboardComponent->InitializeBlackboard(*(Enemy->GetEnemyBehaviorTree()->BlackboardAsset));
}