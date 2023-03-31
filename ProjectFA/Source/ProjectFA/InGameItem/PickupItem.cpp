// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/SphereComponent.h"
#include "ProjectFA/FACharacter/PickupableCharacter.h"

APickupItem::APickupItem()
	:
	PickupItemMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickupItemMeah"))),
	PickupAreaSphere(CreateDefaultSubobject<USphereComponent>(TEXT("PickupAreaSphere")))
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(PickupItemMesh);
	PickupItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	PickupItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	PickupAreaSphere->SetupAttachment(GetRootComponent());
	PickupAreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	PickupAreaSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::PickupAreaBeginOverlap);
	PickupAreaSphere->OnComponentEndOverlap.AddDynamic(this, &APickupItem::PickupAreaEndOverlap);
}

void APickupItem::Destroyed()
{
	Super::Destroyed();

	ItemRemovedFromInventoryEvent.Broadcast(this);
}

void APickupItem::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	if(NewOwner == nullptr)
	{
		ItemRemovedFromInventoryEvent.Clear();
	}
}

void APickupItem::SetItemState(const EItemState State)
{
	ItemState = State;
	switch (ItemState)
	{
	case EItemState::EIS_Equipped:
		PickupItemMesh->SetVisibility(true);
		PickupItemMesh->SetSimulatePhysics(false);
		PickupItemMesh->SetEnableGravity(false);
		PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickupAreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_InInventory:
		PickupItemMesh->SetVisibility(false);
		PickupItemMesh->SetSimulatePhysics(false);
		PickupItemMesh->SetEnableGravity(false);
		PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickupAreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EItemState::EIS_Dropped:
		SetOwner(nullptr);
		PickupItemMesh->SetVisibility(true);
		PickupItemMesh->SetSimulatePhysics(true);
		PickupItemMesh->SetEnableGravity(true);
		PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		PickupItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		PickupAreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	default:
		break;
	}	
}

void APickupItem::PickupAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IPickupableCharacter* PickupableCharacter = Cast<IPickupableCharacter>(OtherActor))
	{
		PickupableCharacter->SetNearbyItem(this);
	}
}

void APickupItem::PickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(IPickupableCharacter* PickupableCharacter = Cast<IPickupableCharacter>(OtherActor))
	{
		PickupableCharacter->UnsetNearbyItem(this);
	}
}