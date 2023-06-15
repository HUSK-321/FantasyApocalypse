// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"
#include "EnemyController.generated.h"

/**
 * 
 */
class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class PROJECTFA_API AEnemyController : public AAIController, public IEnemyControllable
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, Category = "Enemy AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardComponent> EnemyBlackboardComponent;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTreeComponent> EnemyBehaviorTreeComponent;

	UPROPERTY()
	TMap<UObject*, float> TargetObjects;
	
public:
	AEnemyController();
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetControllingEnemyAttack() override;
	virtual void SetEnemyBlackboardValueAsBool(const FName KeyName, bool BoolValue) override;
	virtual void SetEnemyBlackboardValueAsObject(const FName KeyName, UObject* ObjectValue) override;
	virtual void SetEnemyBlackboardValueAsObject(const FName KeyName, UObject* ObjectValue, float Weight) override;
	virtual void SetEnemyBlackboardValueAsVector(const FName KeyName, FVector VectorValue) override;

	virtual bool HaveObject(const UObject* ObjectKeyName) override;

	virtual UObject* GetMostTarget() override;
};