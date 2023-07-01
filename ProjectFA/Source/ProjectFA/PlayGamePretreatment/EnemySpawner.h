// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UBoxComponent;
class AEnemy;

UCLASS()
class PROJECTFA_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEnemy> EnemyClassToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> SpawnArea;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	int32 CurrentEnemyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	int32 MaxSpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	float SpawnInterval;

	FTimerHandle SpawnTimer;
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

private:
	void DoSpawnTimer();
	
	void EnemyDead();
	void SpawnEnemy();
};