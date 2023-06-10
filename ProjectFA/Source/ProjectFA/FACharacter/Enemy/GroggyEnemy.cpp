// Fill out your copyright notice in the Description page of Project Settings.


#include "GroggyEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectFA/FACharacter/PickupableCharacter.h"
#include "ProjectFA/FADictionary/GamePlayCalculator.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"
#include "ProjectFA/InGameItem/Equipable.h"


AGroggyEnemy::AGroggyEnemy()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	SpawnTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxComponent"));
	SpawnTriggerBox->SetupAttachment(GetRootComponent());
	SpawnTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SpawnTriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SpawnTriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SpawnTriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	WeakGroggyGauge = MaxWeakGroggyGauge;
	StrongGroggyGauge = MaxWeakGroggyGauge;
}

void AGroggyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	SetAttackCollision(false);
	GetMesh()->SetRenderInMainPass(false);
	GetMesh()->SetCastShadow(false);
	
	if(HasAuthority() == false)	return;
	
	SpawnTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGroggyEnemy::OnSpawnTriggerBoxOverlap);
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

void AGroggyEnemy::SpawnEnemyByTriggerBox()
{
	SpawnTriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
	if(SpawnMontage == nullptr)	return;
	if(const auto Animinstance = GetMesh()->GetAnimInstance())
	{
		Animinstance->Montage_Play(SpawnMontage);
	}
}

void AGroggyEnemy::DealGroggyDamage(const int32 WeakGroggyDamage, const int32 StrongGroggyDamage, const FVector& DamageCauser)
{
	WeakGroggyGauge -= WeakGroggyDamage;
	StrongGroggyGauge -= StrongGroggyDamage;
	if(StrongGroggyGauge <= 0)
	{
		StrongGroggy();
		return;
	}
	if(WeakGroggyGauge <= 0)
	{
		WeakGroggy(DamageCauser);
	}
}

void AGroggyEnemy::WeakGroggy(const FVector& DamageCauser)
{
	WeakGroggyGauge = MaxWeakGroggyGauge;
	if(WeakGroggyMontage == nullptr)	return;
	
	if(const auto Animinstance = GetMesh()->GetAnimInstance())
	{
		if(Animinstance->Montage_IsPlaying(WeakGroggyMontage) || Animinstance->Montage_IsPlaying(StrongGroggyMontage))	return;
		Animinstance->Montage_Play(WeakGroggyMontage);
		auto ForwardVector = GetActorForwardVector();
		const auto MontageSection = UGamePlayCalculator::GetDirectionSectionName(ForwardVector, GetActorLocation(), DamageCauser);
		Animinstance->Montage_JumpToSection(MontageSection);
	}

	if(const auto EnemyController = GetController<IEnemyControllable>())
	{
		EnemyController->SetEnemyBlackboardValueAsBool(FName("WeakGroggy"), true);
	}
}

void AGroggyEnemy::StrongGroggy()
{
	StrongGroggyGauge = MaxStrongGroggyGauge;
	WeakGroggyGauge = MaxWeakGroggyGauge;
	if(StrongGroggyMontage == nullptr)	return;
	
	if(const auto Animinstance = GetMesh()->GetAnimInstance())
	{
		if(Animinstance->Montage_IsPlaying(StrongGroggyMontage))	return;
		Animinstance->Montage_Play(StrongGroggyMontage);
	}

	if(const auto EnemyController = GetController<IEnemyControllable>())
	{
		EnemyController->SetEnemyBlackboardValueAsBool(FName("StrongGroggy"), true);
	}
}

void AGroggyEnemy::OnSpawnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto PlayableCharacter = Cast<IPickupableCharacter>(OtherActor);
	if(PlayableCharacter == nullptr)	return;
	if(const auto EnemyController = GetController<IEnemyControllable>())
	{
		EnemyController->SetEnemyBlackboardValueAsObject(FName(TEXT("TargetPlayer")), OtherActor);
	}
	
	SpawnEnemyByTriggerBox();
}