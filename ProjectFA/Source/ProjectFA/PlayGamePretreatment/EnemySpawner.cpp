// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectFA/FACharacter/Enemy/Enemy.h"

AEnemySpawner::AEnemySpawner()
	:
	CurrentEnemyCount(0)
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Area"));
	SetRootComponent(SpawnArea);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority() == false)	return;
	DoSpawnTimer();
}

void AEnemySpawner::DoSpawnTimer()
{
	if(CurrentEnemyCount >= MaxSpawnCount)	return;
	if(GetWorld() == nullptr)	return;
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::SpawnEnemy, SpawnInterval, false);
}

void AEnemySpawner::EnemyDead()
{
	CurrentEnemyCount--;
	DoSpawnTimer();
}

void AEnemySpawner::SpawnEnemy()
{
	if(GetWorld() == nullptr || CurrentEnemyCount >= MaxSpawnCount || EnemyClassToSpawn == nullptr)	return;

	const auto RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);
	const FTransform SpawnTransform{ FRotator::ZeroRotator, RandomLocation };
	auto SpawnedEnemy = GetWorld()->SpawnActorDeferred<AEnemy>(EnemyClassToSpawn, SpawnTransform);

	if(SpawnedEnemy)
	{
		SpawnedEnemy->OnEnemyDeadDelegate.AddUObject(this, &AEnemySpawner::EnemyDead);
		SpawnedEnemy->SpawnDefaultController();
		SpawnedEnemy->FinishSpawning(SpawnTransform);
		CurrentEnemyCount++;
	}

	DoSpawnTimer();
}