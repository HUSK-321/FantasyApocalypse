// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/SphereComponent.h"
#include "ProjectFA/FACharacter/Player/PlayableCharacter.h"

APickupItem::APickupItem()
	:
	PickupItemMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickupItemMeah"))),
	PickupAreaSphere(CreateDefaultSubobject<USphereComponent>(TEXT("PickupAreaSphere")))
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(PickupItemMesh);
	PickupItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	PickupAreaSphere->SetupAttachment(GetRootComponent());
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	PickupAreaSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::PickupAreaBeginOverlap);
	PickupAreaSphere->OnComponentEndOverlap.AddDynamic(this, &APickupItem::PickupAreaEndOverlap);
	
}

void APickupItem::PickupAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<APlayableCharacter>(OtherActor);
	if(Player == nullptr)	return;
	Player->SetCurrentPickupItem(this);
}

void APickupItem::PickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Player = Cast<APlayableCharacter>(OtherActor);
	if(Player == nullptr)	return;
	Player->SetCurrentPickupItem(this);
}