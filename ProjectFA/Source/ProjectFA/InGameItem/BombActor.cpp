// Fill out your copyright notice in the Description page of Project Settings.


#include "BombActor.h"
#include "Kismet/GameplayStatics.h"

ABombActor::ABombActor()
{
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	SetRootComponent(BombMesh);
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ABombActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetExplodeTimer();

#if UE_EDITOR
	if(GetWorld())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), DamageOuterRadius, 50, FColor(181, 0, 0), false, TimeToExplode, 0, 1);	
	}
#endif
}

void ABombActor::SetExplodeTimer()
{
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &ABombActor::ExplodeTimerEnd, TimeToExplode);
}

void ABombActor::ExplodeTimerEnd()
{
	ExplodeDamage();
	Destroy();
}

void ABombActor::ExplodeDamage()
{
	if(HasAuthority() == false)	return;
	UGameplayStatics::ApplyRadialDamageWithFalloff(this, BaseDamage, 10.f, GetActorLocation(),DamageInnerRadius, DamageOuterRadius,
		1.f, UDamageType::StaticClass(), TArray<AActor*>(), this, nullptr);
}