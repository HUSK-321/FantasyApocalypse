// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "ProjectFA/PlayGamePretreatment/ItemSpawnable.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;
class USphereComponent;
class UBoxComponent;
class ULootingItemComponent;

DECLARE_MULTICAST_DELEGATE(FOnEnemyDeadDelegate);

UCLASS()
class PROJECTFA_API AEnemy : public AFACharacter, public IItemSpawnable
{
	GENERATED_BODY()

public:
	FOnEnemyDeadDelegate OnEnemyDeadDelegate;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> AttackCollision;
	UPROPERTY(VisibleAnywhere, Category = "Spawn Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULootingItemComponent> LootingItemComponent;
	
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
	FVector PatrolStartPoint;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties", meta = (AllowPrivateAccess = "true"))
	float EnemyBaseDamage;

	UPROPERTY(EditAnywhere, Category = "Spawner Property")
	TArray<FSpawnerInitializeInfo> SpawnCategoryInfo;

	UPROPERTY()
	TSet<AActor*> HittedActors;

	UPROPERTY(ReplicatedUsing = OnRep_AttackToTarget)
	bool bAttackTrigger;

public:
	AEnemy();

	void TriggerAttackToTarget();

	virtual void GenerateInventoryItems() override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Attack")
	void SetAttackCollision(bool bEnabled);

	virtual TArray<FSpawnerInitializeInfo> GetSpawnCategoryPercent() override;
	virtual void SetSpawnItemList(const TArray<APickupItem*>& ItemList) override;

	FORCEINLINE UBehaviorTree* GetEnemyBehaviorTree() const { return EnemyBehaviorTree; }

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnSensingPawn(APawn* OtherPawn);
												
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;
	void SearchEnemyDeadEvent();
	virtual void CurrentHealthChanged() override;
	UFUNCTION()
	void AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
											int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRep_AttackToTarget();
};