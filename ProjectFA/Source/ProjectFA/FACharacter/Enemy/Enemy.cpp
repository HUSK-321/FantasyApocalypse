// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "ProjectFA/FACharacter/Player/PlayableCharacter.h"

AEnemy::AEnemy()
	:
	PawnSensingComponent(CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent")))
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AEnemyController>(GetController());
	
	if(EnemyController)
	{
		const FVector WorldPatrolStartPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolStartPoint);
		const FVector WorldPatrolEndPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolEndPoint);
		DrawDebugSphere(GetWorld(), WorldPatrolStartPoint, 10.f, 12, FColor::Red, true);
		DrawDebugSphere(GetWorld(), WorldPatrolEndPoint, 10.f, 12, FColor::Red, true);
		EnemyController->GetEnemyBlackboardComponent()->SetValueAsVector(TEXT("PatrolStartPoint"), WorldPatrolStartPoint);
		EnemyController->GetEnemyBlackboardComponent()->SetValueAsVector(TEXT("PatrolEndPoint"), WorldPatrolEndPoint);
		EnemyController->RunBehaviorTree(EnemyBehaviorTree);
	}
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSensingPawn);
	OnTakeAnyDamage.AddDynamic(this, &AEnemy::ReceiveDamage);
}

void AEnemy::OnSensingPawn(APawn* OtherPawn)
{
	auto PlayableCharacter = Cast<APlayableCharacter>(OtherPawn);
	if(PlayableCharacter == nullptr)	return;

	EnemyController->GetEnemyBlackboardComponent()->SetValueAsObject(TEXT("TargetPlayer"), PlayableCharacter);
}

void AEnemy::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Enemy Damaged : %f"), Damage);
	if(CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Dead"));
	}
}