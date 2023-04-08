// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingBox.h"
#include "LootingItemComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectFA/InGameItem/PickupItem.h"

ALootingBox::ALootingBox()
	:
	BoxMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"))),
	BoxArea(CreateDefaultSubobject<USphereComponent>(TEXT("Box Area"))),
	ItemGeneratePosition(CreateDefaultSubobject<USceneComponent>(TEXT("Item GeneratePosition"))),
	LootingItemComponent(CreateDefaultSubobject<ULootingItemComponent>(TEXT("Looting Component")))
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(ItemGeneratePosition);

	BoxMesh->SetupAttachment(GetRootComponent());
	BoxArea->SetupAttachment(GetRootComponent());
}

void ALootingBox::BeginPlay()
{
	Super::BeginPlay();

	if(ItemToTestClass == nullptr)	return;
	TArray<APickupItem*> TestItem;
	for(int i = 0; i < 3; i++)
	{
		auto Item = GetWorld()->SpawnActor<APickupItem>(ItemToTestClass);
		TestItem.Add(Item);
	}
	LootingItemComponent->InitializeItemList(TestItem);

	GetWorldTimerManager().SetTimer(TestItemTimerHandle, this, &ALootingBox::CallLooting, 2.f, true);
}

void ALootingBox::CallLooting()
{
	UE_LOG(LogTemp, Warning, TEXT("Generate looting items"));
	LootingItemComponent->GenerateItemsToWorld();
}