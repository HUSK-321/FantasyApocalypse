// Fill out your copyright notice in the Description page of Project Settings.


#include "GroggyEnemy.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectFA/FACharacter/PickupableCharacter.h"
#include "ProjectFA/FADictionary/GamePlayCalculator.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"
#include "ProjectFA/InGameItem/Equipable.h"


AGroggyEnemy::AGroggyEnemy()
{
	SpawnTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxComponent"));
	
	WeakGroggyGauge = MaxWeakGroggyGauge;
	StrongGroggyGauge = MaxWeakGroggyGauge;
}

void AGroggyEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGroggyEnemy, WeakGroggyGauge);
	DOREPLIFETIME(AGroggyEnemy, StrongGroggyGauge);
}

void AGroggyEnemy::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                 AController* InstigatorController, AActor* DamageCauser)
{
	Super::ReceiveDamage(DamagedActor, Damage, DamageType, InstigatorController, DamageCauser);

	const auto GroggyInterface = Cast<IEquipable>(DamageCauser);
	if(GroggyInterface == nullptr)	return;
	
	const int32 WeakGroggyDamage = GroggyInterface->GetWeakGroggyGauge();
	const int32 StrongGroggyDamage = GroggyInterface->GetStrongGroggyGauge();
	const FVector DamageCauserPosition = DamageCauser->GetActorLocation();
	DealGroggyDamage(WeakGroggyDamage, StrongGroggyDamage, DamageCauserPosition);
}

void AGroggyEnemy::DealGroggyDamage(const int32 WeakGroggyDamage, const int32 StrongGroggyDamage, const FVector& DamageCauser)
{
	WeakGroggyGauge -= WeakGroggyDamage;
	StrongGroggyGauge -= StrongGroggyDamage;
	if(StrongGroggyGauge < 0)
	{
		StrongGroggy();
		return;
	}
	if(WeakGroggyGauge < 0)
	{
		WeakGroggy(DamageCauser);
	}
}

void AGroggyEnemy::WeakGroggy(const FVector& DamageCauser)
{
	if(WeakGroggyMontage == nullptr)	return;
	
	WeakGroggyGauge = MaxWeakGroggyGauge;
	
	if(const auto Animinstance = GetMesh()->GetAnimInstance())
	{
		Animinstance->Montage_Play(WeakGroggyMontage);
		auto ForwardVector = GetActorForwardVector();
		const auto MontageSection = UGamePlayCalculator::GetDirectionSectionName(ForwardVector, GetActorLocation(), DamageCauser);
		Animinstance->Montage_JumpToSection(MontageSection);
	}
}

void AGroggyEnemy::StrongGroggy()
{
	StrongGroggyGauge = MaxStrongGroggyGauge;
	WeakGroggyGauge = MaxWeakGroggyGauge;
	
	if(StrongGroggyMontage == nullptr)	return;
}

void AGroggyEnemy::OnSpawnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto PlayableCharacter = Cast<IPickupableCharacter>(OtherActor);
	if(PlayableCharacter == nullptr)	return;
	const auto EnemyController = GetController<IEnemyControllable>();
	if(EnemyController == nullptr)	return;

	EnemyController->SetEnemyBlackboardValueAsBool(FName(TEXT("Spawn")), true);
	// Play Spawn Animation
}