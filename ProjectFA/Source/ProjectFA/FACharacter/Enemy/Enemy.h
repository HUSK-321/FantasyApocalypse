// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;
class AEnemyController;

UCLASS()
class PROJECTFA_API AEnemy : public AFACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	TObjectPtr<AEnemyController> EnemyController;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	FVector PatrolStartPoint;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	FVector PatrolEndPoint;

public:
	
	AEnemy();

	FORCEINLINE UBehaviorTree* GetEnemyBehaviorTree() const { return EnemyBehaviorTree; }

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSensingPawn(APawn* OtherPawn);
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;
};
