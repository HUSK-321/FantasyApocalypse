// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "WeaponItemDataAsset.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectFA/FADictionary/GamePlayCalculator.h"
#include "ProjectFA/FAInterfaces/Controller/ItemRPCableController.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetRootComponent());
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackCollision->bHiddenInGame = false;
	AttackCollision->SetIsReplicated(true);
}

void AWeapon::SetItemPropertyFromDataAsset(const UItemDataAsset* DataAsset)
{
	Super::SetItemPropertyFromDataAsset(DataAsset);
	const auto WeaponDataAsset = Cast<UWeaponItemDataAsset>(DataAsset);
	if(WeaponDataAsset == nullptr)	return;

	WeaponSkeletal = WeaponDataAsset->WeaponSkeletalMesh;
	OnRep_WeaponSkeletal();
	WeaponInfo.WeaponType = WeaponDataAsset->WeaponType;
	WeaponInfo.DamageTypeClass = WeaponDataAsset->DamageTypeClass;
	WeaponInfo.WeakGroggyGauge = WeaponDataAsset->WeakGroggyGauge;
	WeaponInfo.StrongGroggyGauge = WeaponDataAsset->StrongGroggyGauge;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority() == false)	return;
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::AttackCollisionOnOverlapBegin);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponInfo);
	DOREPLIFETIME_CONDITION(AWeapon, WeaponSkeletal, COND_SkipOwner);
}

void AWeapon::OnRep_WeaponSkeletal()
{
	WeaponMesh->SetSkeletalMesh(WeaponSkeletal);
}

void AWeapon::SetItemVisibilityByState()
{
	Super::SetItemVisibilityByState();
	
	switch (ItemState)
	{
	case EItemState::EIS_Initial:
		WeaponMesh->SetVisibility(false);

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_Equipped:
		WeaponMesh->SetVisibility(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_InInventory:
		WeaponMesh->SetVisibility(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_Dropped:
		WeaponMesh->SetVisibility(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	default:
		break;
	}
}

void AWeapon::DropItem(const float DropImpulsePower)
{
	UnEquip();
	GetPlayerDamageProperty.Clear();
	Super::DropItem(DropImpulsePower);
}

FName AWeapon::GetNormalAttackMontageSectionName() const
{
	switch (WeaponInfo.WeaponType)
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
	if(GetAttachParentActor() == nullptr)	return;
	
	const FDetachmentTransformRules DetachRules{ EDetachmentRule::KeepWorld, true };
	this->DetachFromActor(DetachRules);
	SetItemState(EItemState::EIS_InInventory);
	UnEquipEvent.Broadcast(this);
}

void AWeapon::AttackStart_Implementation()
{
	HittedActors.AddUnique(GetOwner());
	HittedActors.AddUnique(this);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AWeapon::AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(WeaponInfo.DamageTypeClass == nullptr || HittedActors.Contains(OtherActor))	return;
	
	const APawn* AttackingPawn = Cast<APawn>(GetOwner());
	if(AttackingPawn == nullptr)	return;
	const auto AttackingInstigator = AttackingPawn->GetController();
	if(AttackingInstigator == nullptr)	return;

	float Damage = ItemInfo.ItemPowerAmount;
	if(GetPlayerDamageProperty.IsBound())
	{
		Damage = UGamePlayCalculator::CalculateWeaponDamage(ItemInfo.ItemPowerAmount, GetPlayerDamageProperty.Execute());
	}
	UGameplayStatics::ApplyDamage(OtherActor, Damage, AttackingInstigator, this, WeaponInfo.DamageTypeClass);
	HittedActors.AddUnique(OtherActor);
}

void AWeapon::AttackEnd_Implementation()
{
	HittedActors.Empty();
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::SetEquipItemEvent(const FEquipItemEvent& Event)
{
	EquipItemEvent = Event;
}

void AWeapon::SetUnEquipEvent(const FEquipItemEvent& Event)
{
	UnEquipEvent = Event;	
}

void AWeapon::SetPlayerDamagePropertyDelegate(const FGetPlayerDamagePropertyDelegate& Event)
{
	GetPlayerDamageProperty = Event;
}

void AWeapon::InventoryAction_Implementation()
{
	EquipItemEvent.Broadcast(this);
}

void AWeapon::RemoveFromInventoryAction_Implementation()
{
	const auto OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr)	return;
	if(const auto OwnerController = OwnerPawn->GetController<IItemRPCableController>())
	{
		OwnerController->DropItem(this);
	}
}