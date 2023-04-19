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
}

void AWeapon::AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const APawn* AttackingPawn = Cast<APawn>(GetOwner());
	if(OtherActor == GetOwner() || AttackingPawn == nullptr || DamageTypeClass == nullptr)	return;
	const auto AttackingInstigator = AttackingPawn->GetController();
	if(AttackingInstigator == nullptr)	return;
	
	UGameplayStatics::ApplyDamage(OtherActor, ItemPowerAmount, AttackingInstigator, this, DamageTypeClass);
	SetAttackCollision(false);
}

FName AWeapon::GetNormalAttackMontageSectionName() const
{
	switch (WeaponType)
	{
	case EWeaponType::EWT_OneHandSword:
		return FName(TEXT("OneHandSword"));
	case EWeaponType::EWT_MagicStaff:
		return FName(TEXT("MagicStaff"));
	default:
		return FName(TEXT("Default"));
	}
}

void AWeapon::UnEquip()
{
	const FDetachmentTransformRules DetachRules{ EDetachmentRule::KeepWorld, true };
	PickupItemMesh->DetachFromComponent(DetachRules);
	SetItemState(EItemState::EIS_InInventory);
	UnEquipEvent.Broadcast(this);
}

void AWeapon::SetAttackCollision(bool bEnable)
{
	if(AttackCollision == nullptr)	return;
	const auto CollisionResponseToPawn = (bEnable) ? ECollisionResponse::ECR_Overlap : ECollisionResponse::ECR_Ignore;
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, CollisionResponseToPawn);
}

void AWeapon::SetEquipItemEvent(const FEquipItemEvent& Event)
{
	EquipItemEvent.Clear();
	EquipItemEvent = Event;
}

void AWeapon::SetUnEquipEvent(const FEquipItemEvent& Event)
{
	UnEquipEvent = Event;	
}

void AWeapon::InventoryAction_Implementation()
{
	EquipItemEvent.Broadcast(this);
}

void AWeapon::RemoveFromInventoryAction_Implementation()
{
	UnEquip();
	DropItem();
}