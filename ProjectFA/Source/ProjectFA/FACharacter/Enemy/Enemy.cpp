// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "ProjectFA/FACharacter/PickupableCharacter.h"
#include "ProjectFA/FADictionary/FACoreDelegates.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"
#include "ProjectFA/Interactable/Looting/LootingItemComponent.h"

AEnemy::AEnemy()
	:
	PawnSensingComponent(CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"))),
	AttackCollision(CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"))),
	LootingItemComponent(CreateDefaultSubobject<ULootingItemComponent>(TEXT("Looting Item Component"))),
	EnemyBaseDamage(10.f),
	bAttackTrigger(false)
{
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	AttackCollision->SetupAttachment(GetMesh(), FName("AttackCollisionSocket"));
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	SetAttackCollision(false);
	
	if(HasAuthority() == false || IsValid(GetController()) == false)	return;

	if(const auto EnemyController = Cast<IEnemyControllable>(GetController()))
	{
		PatrolStartPoint = GetActorLocation();
		DrawDebugSphere(GetWorld(), PatrolStartPoint, 10.f, 12, FColor::Red, false, 2.f);
		EnemyController->SetEnemyBlackboardValueAsVector(TEXT("PatrolStartPoint"), PatrolStartPoint);
		if(const auto AIController = Cast<AAIController>(GetController()))
		{
			AIController->RunBehaviorTree(EnemyBehaviorTree);
		}
	}
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSensingPawn);
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackCollisionOnOverlapBegin);
	OnTakeAnyDamage.AddDynamic(this, &AEnemy::ReceiveDamage);
}

void AEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy, bAttackTrigger);
}

void AEnemy::OnSensingPawn(APawn* OtherPawn)
{
	const auto PlayableCharacter = Cast<IPickupableCharacter>(OtherPawn);
	if(PlayableCharacter == nullptr)	return;
	const auto EnemyController = GetController<IEnemyControllable>();
	if(EnemyController == nullptr || EnemyController->HaveObject(OtherPawn))	return;
	
	EnemyController->SetEnemyBlackboardValueAsObject(TEXT("TargetPlayer"), OtherPawn, 1.f);
}

void AEnemy::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                           AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	OnRep_CurrentHealthChanged();

#if WITH_EDITOR
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy Damaged : %f"), Damage));
#endif

	const auto EnemyController = GetController<IEnemyControllable>();
	if(EnemyController == nullptr || InstigatorController == nullptr)	return;
	
	EnemyController->SetEnemyBlackboardValueAsObject(TEXT("TargetPlayer"), InstigatorController->GetPawn(), 3.f);
}

void AEnemy::SearchEnemyDeadEvent()
{
	OnEnemyDeadDelegate.Broadcast();
	// 서버에서 처치자에게만 아래 내용이 불릴 수 있게 처리하기
	FACoreDelegates::OnEnemyDestroyed.Broadcast(this);
}

void AEnemy::CurrentHealthChanged()
{
	if(CurrentHealth <= 0 && NowInDeadProcess() == false)
	{
		SearchEnemyDeadEvent();
		CharacterDead();
	}
}

void AEnemy::SetAttackCollision(bool bEnabled)
{
	if(bEnabled == false)
	{
		HittedActors.Empty();
		HittedActors.Add(this);
	}
	const auto CollisionEnabled = bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;
	AttackCollision->SetCollisionEnabled(CollisionEnabled);
}

void AEnemy::AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == this || DamageTypeClass == nullptr || HittedActors.Contains(OtherActor) || GetController() == nullptr)	return;

	HittedActors.Add(OtherActor);
	UGameplayStatics::ApplyDamage(OtherActor, EnemyBaseDamage, GetController(), this, DamageTypeClass);

	if(const auto GameCharacter = Cast<AFACharacter>(OtherActor))
	{
		GameCharacter->MulticastPlayHitEffect(AttackCollision->GetComponentLocation());
	}
}

void AEnemy::SetSpawnItemList(const TArray<APickupItem*>& ItemList)
{
	LootingItemComponent->InitializeItemList(ItemList);
}

bool AEnemy::IsItemSpawned()
{
	return (LootingItemComponent) ? LootingItemComponent->IsItemSpawned() : true;
}

TArray<FSpawnerInitializeInfo> AEnemy::GetSpawnCategoryPercent()
{
	return SpawnCategoryInfo;
}

void AEnemy::TriggerAttackToTarget()
{
	bAttackTrigger = !bAttackTrigger;
	OnRep_AttackToTarget();
}

void AEnemy::GenerateInventoryItems()
{
	LootingItemComponent->GenerateItemsToWorld();
}

void AEnemy::OnRep_AttackToTarget()
{
	PlayNormalAttackMontage();
}
