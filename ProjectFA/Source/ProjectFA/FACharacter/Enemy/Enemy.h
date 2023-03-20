// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;
class AEnemyController;
class USphereComponent;
class UBoxComponent;

UCLASS()
class PROJECTFA_API AEnemy : public AFACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> AttackSphere;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> AttackCollision;
	
	TObjectPtr<AEnemyController> EnemyController;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	FVector PatrolStartPoint;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	FVector PatrolEndPoint;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	TSubclassOf<UDamageType> DamageTypeClass;

public:
	
	AEnemy();

	UFUNCTION(BlueprintCallable, Category = "Enemy Attack")
	void SetAttackCollision(bool bEnabled);

	FORCEINLINE UBehaviorTree* GetEnemyBehaviorTree() const { return EnemyBehaviorTree; }

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSensingPawn(APawn* OtherPawn);
	UFUNCTION()
	void AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex);
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	void PlayNormalAttackMontage();
	UFUNCTION()
	void AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
											int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};