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
	GetMesh()->bPauseAnims = true;
	
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
	GetMesh()->bPauseAnims = false;
	SpawnTriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
	if(SpawnMontage == nullptr)	return;
	if(const auto Animinstance = GetMesh()->GetAnimInstance())
	{
		Animinstance->Montage_Play(SpawnMontage);
		
		Animinstance->OnMontageEnded.Clear();
		FOnMontageEnded MontageEnded;
		MontageEnded.BindLambda([&](UAnimMontage*, bool)
		{
			if(const auto EnemyController = GetController<IEnemyControllable>())
			{
				EnemyController->SetEnemyBlackboardValueAsBool(FName("Spawned"), true);
			}
		});
		Animinstance->Montage_SetEndDelegate(MontageEnded);
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
	if(WeakGroggyMontage == nullptr)	return;
	if(WeakGroggyMontagePlay(DamageCauser) == false) return;

	if(const auto EnemyController = GetController<IEnemyControllable>())
	{
		EnemyController->SetEnemyBlackboardValueAsBool(FName("WeakGroggy"), true);
	}
	
	WeakGroggyGauge = MaxWeakGroggyGauge;
}

void AGroggyEnemy::StrongGroggy()
{
	if(StrongGroggyMontage == nullptr)	return;
	if(StrongGroggyMontagePlay() == false) return;

	if(const auto EnemyController = GetController<IEnemyControllable>())
	{
		EnemyController->SetEnemyBlackboardValueAsBool(FName("StrongGroggy"), true);
	}
	
	StrongGroggyGauge = MaxStrongGroggyGauge;
	WeakGroggyGauge = MaxWeakGroggyGauge;
}

void AGroggyEnemy::PlayNormalAttackMontage(FName NormalAttackSectionName)
{
	if(CurrentHealth/MaxHealth < 0.5f)
	{
		NormalAttackSectionName = FName("DyingAttack");
	}
	Super::PlayNormalAttackMontage(NormalAttackSectionName);
}

bool AGroggyEnemy::WeakGroggyMontagePlay(const FVector& DamageCauser)
{
	if(const auto Animinstance = GetMesh()->GetAnimInstance())
	{
		if(Animinstance->Montage_IsPlaying(WeakGroggyMontage) || Animinstance->Montage_IsPlaying(StrongGroggyMontage))	return false;
		auto ForwardVector = GetActorForwardVector();
		const auto MontageSection = UGamePlayCalculator::GetDirectionSectionName(ForwardVector, GetActorLocation(), DamageCauser);
		MulticastWeakGroggyMontagePlay(MontageSection);

		Animinstance->OnMontageEnded.Clear();
		FOnMontageEnded MontageEnded;
		MontageEnded.BindLambda([&](UAnimMontage*, bool)
		{
			if(const auto EnemyController = GetController<IEnemyControllable>())
			{
				EnemyController->SetEnemyBlackboardValueAsBool(FName("WeakGroggy"), false);
			}
		});
		Animinstance->Montage_SetEndDelegate(MontageEnded);
	}
	return true;
}

bool AGroggyEnemy::StrongGroggyMontagePlay()
{
	if(const auto Animinstance = GetMesh()->GetAnimInstance())
	{
		if(Animinstance->Montage_IsPlaying(StrongGroggyMontage))	return false;
		MulticastStrongGroggyMontagePlay();

		Animinstance->OnMontageEnded.Clear();
		FOnMontageEnded MontageEnded;
		MontageEnded.BindLambda([&](UAnimMontage*, bool)
		{
			if(const auto EnemyController = GetController<IEnemyControllable>())
			{
				EnemyController->SetEnemyBlackboardValueAsBool(FName("StrongGroggy"), false);
			}
		});
		Animinstance->Montage_SetEndDelegate(MontageEnded);
	}
	return true;
}

void AGroggyEnemy::MulticastWeakGroggyMontagePlay_Implementation(FName MontageSection)
{
	if(GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr)	return;

	GetMesh()->GetAnimInstance()->Montage_Play(WeakGroggyMontage);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(MontageSection);
}

void AGroggyEnemy::MulticastStrongGroggyMontagePlay_Implementation()
{
	if(GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr)	return;

	GetMesh()->GetAnimInstance()->Montage_Play(StrongGroggyMontage);
}

void AGroggyEnemy::OnSpawnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto PlayableCharacter = Cast<IPickupableCharacter>(OtherActor);
	if(PlayableCharacter == nullptr)	return;
	if(const auto EnemyController = GetController<IEnemyControllable>())
	{
		EnemyController->SetEnemyBlackboardValueAsObject(FName(TEXT("TargetPlayer")), OtherActor, 2.f);
	}

	MulticastSpawnEnemy();
}

void AGroggyEnemy::MulticastSpawnEnemy_Implementation()
{
	SpawnEnemyByTriggerBox();
}