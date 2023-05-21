// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "ProjectFA/FACharacter/PickupableCharacter.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"
#include "ProjectFA/Interactable/Looting/LootingItemComponent.h"

AEnemy::AEnemy()
	:
	PawnSensingComponent(CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"))),
	AttackSphere(CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"))),
	AttackCollision(CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"))),
	LootingItemComponent(CreateDefaultSubobject<ULootingItemComponent>(TEXT("Looting Item Component"))),
	bAttackTrigger(false)
{
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = false;

	AttackSphere->SetupAttachment(GetRootComponent());
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
	
	if(HasAuthority() == false || IsValid(GetController()) == false)	return;

	const auto EnemyController = Cast<IEnemyControllable>(GetController());
	if(EnemyController)
	{
		const FVector WorldPatrolStartPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolStartPoint);
		const FVector WorldPatrolEndPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolEndPoint);
		DrawDebugSphere(GetWorld(), WorldPatrolStartPoint, 10.f, 12, FColor::Red, true);
		DrawDebugSphere(GetWorld(), WorldPatrolEndPoint, 10.f, 12, FColor::Red, true);
		EnemyController->SetEnemyBlackboardValueAsVector(TEXT("PatrolStartPoint"), WorldPatrolStartPoint);
		EnemyController->SetEnemyBlackboardValueAsVector(TEXT("PatrolEndPoint"), WorldPatrolEndPoint);

		if(const auto AIController = Cast<AAIController>(GetController()))
		{
			AIController->RunBehaviorTree(EnemyBehaviorTree);
		}
	}
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSensingPawn);
	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackSphereOnOverlapBegin);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AttackSphereOnOverlapEnd);
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
	if(EnemyController == nullptr)	return;

	EnemyController->SetEnemyBlackboardValueAsObject(TEXT("TargetPlayer"), OtherPawn);
}

void AEnemy::AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto PlayableCharacter = Cast<IPickupableCharacter>(OtherActor);
	if(PlayableCharacter == nullptr)	return;
	const auto EnemyController = GetController<IEnemyControllable>();
	if(EnemyController == nullptr)	return;
	
	EnemyController->SetEnemyBlackboardValueAsBool(TEXT("TargetPlayerIsNear"), true);
}

void AEnemy::AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto PlayableCharacter = Cast<IPickupableCharacter>(OtherActor);
	if(PlayableCharacter == nullptr)	return;
	const auto EnemyController = GetController<IEnemyControllable>();
	if(EnemyController == nullptr)	return;
	
	EnemyController->SetEnemyBlackboardValueAsBool(TEXT("TargetPlayerIsNear"), false);
}

void AEnemy::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                           AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	OnRep_CurrentHealthChanged();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy Damaged : %f"), Damage));
}

void AEnemy::CurrentHealthChanged()
{
	if(CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Dead"));
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
	if(OtherActor == this || DamageTypeClass == nullptr || HittedActors.Contains(OtherActor))	return;
	const auto AttackingInstigator = GetController();
	if(AttackingInstigator == nullptr)	return;

	HittedActors.Add(OtherActor);
	UGameplayStatics::ApplyDamage(OtherActor, 10.f, AttackingInstigator, this, DamageTypeClass);
}

void AEnemy::SetSpawnItemList(const TArray<APickupItem*>& ItemList)
{
	LootingItemComponent->InitializeItemList(ItemList);
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
