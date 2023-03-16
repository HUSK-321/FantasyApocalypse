// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
	:
	AttackCollision(CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision")))
{
	AttackCollision->SetupAttachment(GetRootComponent());

	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::AttackCollisionOnOverlapBegin);
	SetWeaponAttackCollision(false);
}

void AWeapon::AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const APawn* AttackingPawn = Cast<APawn>(GetOwner());
	if(AttackingPawn == nullptr || DamageTypeClass == nullptr)	return;
	const auto AttackingInstigator = AttackingPawn->GetController();
	if(AttackingInstigator == nullptr)	return;
	
	UGameplayStatics::ApplyDamage(OtherActor, ItemPowerAmount, AttackingInstigator, this, DamageTypeClass);
}

void AWeapon::SetWeaponAttackCollision(bool bEnable)
{
	const auto CollisionResponseToPawn = (bEnable) ? ECollisionResponse::ECR_Overlap : ECollisionResponse::ECR_Ignore;
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, CollisionResponseToPawn);
}

void AWeapon::SetWeaponState(const EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		PickupItemMesh->SetSimulatePhysics(false);
		PickupItemMesh->SetEnableGravity(false);
		PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		SetWeaponAttackCollision(false);
		break;
		
	case EWeaponState::EWS_Dropped:
		PickupItemMesh->SetSimulatePhysics(true);
		PickupItemMesh->SetEnableGravity(true);
		PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		PickupItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		SetWeaponAttackCollision(false);
		break;
	default:
		break;
	}	
}