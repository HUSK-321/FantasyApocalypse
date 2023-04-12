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
	PickupAreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	PickupAreaSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::PickupAreaBeginOverlap);
	PickupAreaSphere->OnComponentEndOverlap.AddDynamic(this, &APickupItem::PickupAreaEndOverlap);
}

void APickupItem::Destroyed()
{
	ItemDroppedEvent.Broadcast(this);
	Super::Destroyed();
}

void APickupItem::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	if(NewOwner == nullptr)
	{
		ItemDroppedEvent.Clear();
	}
}

void APickupItem::SetItemState(const EItemState State)
{
	ItemState = State;
	switch (ItemState)
	{
	case EItemState::EIS_Initial:
		SetOwner(nullptr);
		PickupItemMesh->SetSimulatePhysics(false);
		PickupItemMesh->SetEnableGravity(false);
		PickupItemMesh->SetVisibility(true);
		PickupItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PickupAreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PickupAreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		break;
		
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
		PickupItemMesh->SetVisibility(true);
		PickupItemMesh->SetSimulatePhysics(true);
		PickupItemMesh->SetEnableGravity(true);
		PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		PickupItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		
		PickupAreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	default:
		break;
	}	
}

void APickupItem::DropItem(bool bFromPlayerInventory)
{
	if(GetOwner() == nullptr)	return;
	
	SetActorLocation(GetOwner()->GetActorLocation());
	const FRotator MeshRotation{ 0.f, GetOwner()->GetActorRotation().Yaw, 0.f };
	SetActorRotation(MeshRotation, ETeleportType::TeleportPhysics);
	SetItemState(EItemState::EIS_Dropped);
	
	FVector ImpulseDirection = GetActorForwardVector();
	const float RandomRotation = FMath::FRandRange(-40.f, 40.f);
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector::UpVector);
	PickupItemMesh->AddImpulse(ImpulseDirection * 1000.f);
	
	GetWorldTimerManager().SetTimer(DropTimer, this, &APickupItem::DropEnd, 2.0f);

	(bFromPlayerInventory) ? ItemDroppedEvent.Broadcast(this) : ItemDroppedEvent.Clear();
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

void APickupItem::DropEnd()
{
	SetItemState(EItemState::EIS_Initial);
}