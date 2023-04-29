// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "ProjectFA/PlayGamePretreatment/ItemSpawnable.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;
class AEnemyController;
class USphereComponent;
class UBoxComponent;
class ULootingItemComponent;

UCLASS()
class PROJECTFA_API AEnemy : public AFACharacter, public IItemSpawnable
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> AttackSphere;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> AttackCollision;
	UPROPERTY(VisibleAnywhere, Category = "Spawn Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULootingItemComponent> LootingItemComponent;
	
	TObjectPtr<AEnemyController> EnemyController;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	FVector PatrolStartPoint;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	FVector PatrolEndPoint;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, Category = "Spawner Property")
	TArray<FSpawnerInitializeInfo> SpawnCategoryInfo;

	UPROPERTY()
	TSet<AActor*> HittedActors;

public:
	
	AEnemy();

	virtual void AfterDeath() override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Attack")
	void SetAttackCollision(bool bEnabled);

	virtual TArray<FSpawnerInitializeInfo> GetSpawnCategoryPercent() override;
	virtual void SetSpawnItemList(const TArray<APickupItem*>& ItemList) override;

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
	UFUNCTION()
	void AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
											int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};