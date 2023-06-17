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

void AEnemyController::SetControllingEnemyAttack()
{
	const auto EnemyCharacter = Cast<AEnemy>(GetPawn());
	if(EnemyCharacter == nullptr)	return;
	EnemyCharacter->TriggerAttackToTarget();
}

void AEnemyController::SetEnemyBlackboardValueAsBool(const FName KeyName, bool BoolValue)
{
	EnemyBlackboardComponent->SetValueAsBool(KeyName, BoolValue);
}

void AEnemyController::SetEnemyBlackboardValueAsObject(const FName KeyName, UObject* ObjectValue, float Weight)
{
	if(ObjectValue != nullptr)
	{
		if(Weight <= KINDA_SMALL_NUMBER && TargetObjects.Contains(ObjectValue))
		{
			TargetObjects.Remove(ObjectValue);
		}
		else
		{
			(TargetObjects.Contains(ObjectValue) == false) ? TargetObjects.Add(ObjectValue, Weight) : TargetObjects[ObjectValue] += Weight;
		}
	}
	
	SetEnemyBlackboardValueAsObject(KeyName, GetMostTarget());
}

void AEnemyController::SetEnemyBlackboardValueAsObject(const FName KeyName, UObject* ObjectValue)
{
	EnemyBlackboardComponent->SetValueAsObject(KeyName, ObjectValue);
}

void AEnemyController::SetEnemyBlackboardValueAsVector(const FName KeyName, FVector VectorValue)
{
	EnemyBlackboardComponent->SetValueAsVector(KeyName, VectorValue);
}

bool AEnemyController::HaveObject(const UObject* ObjectKeyName)
{
	return TargetObjects.Contains(ObjectKeyName);
}

UObject* AEnemyController::GetMostTarget()
{
	TargetObjects.ValueSort([](const float& A, const float& B){ return A > B; });
	for(const auto& KeyValue : TargetObjects)
	{
		if(KeyValue.Key == nullptr || KeyValue.Value < KINDA_SMALL_NUMBER)	continue;
		return KeyValue.Key;
	}
	return nullptr;
}
