// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
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

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSensingPawn);
	OnTakeAnyDamage.AddDynamic(this, &AEnemy::ReceiveDamage);
}

void AEnemy::OnSensingPawn(APawn* OtherPawn)
{
	auto PlayableCharacter = Cast<APlayableCharacter>(OtherPawn);
	if(PlayableCharacter == nullptr)	return;

	TargetActor = PlayableCharacter;
}

void AEnemy::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	if(CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Dead"));
	}
}

