// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyController.generated.h"

/**
 * 
 */
class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class PROJECTFA_API AEnemyController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, Category = "Enemy AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardComponent> EnemyBlackboardComponent;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTreeComponent> EnemyBehaviorTreeComponent;
	
public:
	AEnemyController();
	virtual void OnPossess(APawn* InPawn) override;

	void SetControllingEnemyAttack();

	FORCEINLINE UBlackboardComponent* GetEnemyBlackboardComponent() const { return EnemyBlackboardComponent; }
	
};