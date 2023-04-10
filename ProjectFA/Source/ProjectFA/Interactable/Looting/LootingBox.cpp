// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingBox.h"
#include "LootingItemComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectFA/FACharacter/InteractableCharacter.h"
#include "ProjectFA/InGameItem/PickupItem.h"

ALootingBox::ALootingBox()
	:
	BoxMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"))),
	BoxArea(CreateDefaultSubobject<USphereComponent>(TEXT("Box Area"))),
	ItemGeneratePosition(CreateDefaultSubobject<USceneComponent>(TEXT("Item GeneratePosition"))),
	LootingItemComponent(CreateDefaultSubobject<ULootingItemComponent>(TEXT("Looting Component"))),
	TimeToSearch(2.f)
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(ItemGeneratePosition);

	BoxMesh->SetupAttachment(GetRootComponent());
	
	BoxArea->SetupAttachment(GetRootComponent());
	BoxArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ALootingBox::SearchActor(const float SearchTime)
{
	TimeToSearch -= SearchTime;
	if(TimeToSearch <= 0)
	{
		LootingItemComponent->GenerateItemsToWorld();
		Destroy();
	}
}

void ALootingBox::FindItem_Implementation(const float SearchTime)
{
	TimeToSearch -= SearchTime;
	if(TimeToSearch <= 0)
	{
		LootingItemComponent->GenerateItemsToWorld();
		Destroy();
	}
}

void ALootingBox::BeginPlay()
{
	Super::BeginPlay();

	BoxArea->OnComponentBeginOverlap.AddDynamic(this, &ALootingBox::LootAreaBeginOverlap);
	BoxArea->OnComponentEndOverlap.AddDynamic(this, &ALootingBox::LootAreaEndOverlap);

	if(ItemToTestClass == nullptr)	return;
	TArray<APickupItem*> TestItem;
	for(int i = 0; i < 3; i++)
	{
		auto Item = GetWorld()->SpawnActor<APickupItem>(ItemToTestClass);
		TestItem.Add(Item);
	}
	LootingItemComponent->InitializeItemList(TestItem);
}

void ALootingBox::LootAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(const auto InteractableCharacter = Cast<IInteractableCharacter>(OtherActor))
	{
		InteractableCharacter->SetInteractingActor(this);
	}
}

void ALootingBox::LootAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(const auto InteractableCharacter = Cast<IInteractableCharacter>(OtherActor))
	{
		InteractableCharacter->SetInteractingActor(nullptr);
	}
}