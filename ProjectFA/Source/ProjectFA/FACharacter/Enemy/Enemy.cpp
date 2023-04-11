// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "ProjectFA/FACharacter/Player/PlayableCharacter.h"
#include "ProjectFA/Interactable/Looting/LootingItemComponent.h"

AEnemy::AEnemy()
	:
	PawnSensingComponent(CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"))),
	AttackSphere(CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"))),
	AttackCollision(CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"))),
	LootingItemComponent(CreateDefaultSubobject<ULootingItemComponent>(TEXT("Looting Item Component"))),
	DissolveTimeline(CreateDefaultSubobject<UTimelineComponent>(TEXT("Dissolve Timeline Component")))
{
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
	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackSphereOnOverlapBegin);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AttackSphereOnOverlapEnd);
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackCollisionOnOverlapBegin);
	OnTakeAnyDamage.AddDynamic(this, &AEnemy::ReceiveDamage);
}

void AEnemy::OnSensingPawn(APawn* OtherPawn)
{
	const auto PlayableCharacter = Cast<APlayableCharacter>(OtherPawn);
	if(PlayableCharacter == nullptr)	return;

	EnemyController->GetEnemyBlackboardComponent()->SetValueAsObject(TEXT("TargetPlayer"), PlayableCharacter);
}

void AEnemy::AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto PlayableCharacter = Cast<APlayableCharacter>(OtherActor);
	if(PlayableCharacter == nullptr)	return;

	EnemyController->GetEnemyBlackboardComponent()->SetValueAsBool(TEXT("TargetPlayerIsNear"), true);
}

void AEnemy::AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto PlayableCharacter = Cast<APlayableCharacter>(OtherActor);
	if(PlayableCharacter == nullptr)	return;
	
	EnemyController->GetEnemyBlackboardComponent()->SetValueAsBool(TEXT("TargetPlayerIsNear"), false);
}

void AEnemy::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                           AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy Damaged : %f"), Damage));
	if(CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Dead"));
		CharacterDead();
	}
}

void AEnemy::SetAttackCollision(bool bEnabled)
{
	const auto CollisionEnabled = bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;
	AttackCollision->SetCollisionEnabled(CollisionEnabled);
}

void AEnemy::AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == this || DamageTypeClass == nullptr)	return;
	const auto AttackingInstigator = GetController();
	if(AttackingInstigator == nullptr)	return;
	
	UGameplayStatics::ApplyDamage(OtherActor, 10.f, AttackingInstigator, this, DamageTypeClass);
}

void AEnemy::AfterDeath()
{
	LootingItemComponent->GenerateItemsToWorld();
	
	StartDeadDissolve();
}

void AEnemy::StartDeadDissolve()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
	GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Disslove"), -0.55f);

	if(GetMesh() == nullptr || DissolveCurve == nullptr || DissolveTimeline == nullptr)	return;

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	FOnTimelineFloat DissolveTrack;
	DissolveTrack.BindDynamic(this, &AEnemy::UpdateMaterialDissolve);
	DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);

	FOnTimelineEvent TimelineEndEvent;
	TimelineEndEvent.BindDynamic(this, &AEnemy::AfterDeadDissolve);
	DissolveTimeline->SetTimelineFinishedFunc(TimelineEndEvent);
	DissolveTimeline->Play();
}

void AEnemy::UpdateMaterialDissolve(float DissolveTime)
{
	if(DynamicDissolveMaterialInstance == nullptr)	return;
	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveTime);
}

void AEnemy::AfterDeadDissolve()
{
	Destroy();
}

void AEnemy::SetSpawnItemList(const TArray<APickupItem*>& ItemList)
{
	LootingItemComponent->InitializeItemList(ItemList);
}

const int32 AEnemy::GetSpawnIndex()
{
	return SpawnIndex;
}