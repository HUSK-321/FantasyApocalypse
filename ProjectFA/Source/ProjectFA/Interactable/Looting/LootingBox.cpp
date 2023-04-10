// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingBox.h"
#include "LootingItemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "ProjectFA/FACharacter/InteractableCharacter.h"
#include "ProjectFA/HUD/InteractionWidget/ItemLootingProgressWidget.h"
#include "ProjectFA/InGameItem/PickupItem.h"

ALootingBox::ALootingBox()
	:
	BoxMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"))),
	BoxArea(CreateDefaultSubobject<USphereComponent>(TEXT("Box Area"))),
	ItemGeneratePosition(CreateDefaultSubobject<USceneComponent>(TEXT("Item GeneratePosition"))),
	LootingItemComponent(CreateDefaultSubobject<ULootingItemComponent>(TEXT("Looting Component"))),
	ProgressWidgetComponent(CreateDefaultSubobject<UWidgetComponent>(TEXT("Progress Widget"))),
	DissolveTimeline(CreateDefaultSubobject<UTimelineComponent>(TEXT("Dissolve Timeline Component"))),
	MaxTimeToSearch(2.f), TimeToSearch(0.f)
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(ItemGeneratePosition);

	BoxMesh->SetupAttachment(GetRootComponent());
	
	BoxArea->SetupAttachment(GetRootComponent());
	BoxArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ProgressWidgetComponent->SetupAttachment(GetRootComponent());
}

void ALootingBox::BeginPlay()
{
	Super::BeginPlay();

	ProgressWidgetComponent->SetVisibility(false);
	ProgressWidget = Cast<UItemLootingProgressWidget>(ProgressWidgetComponent->GetUserWidgetObject());

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

void ALootingBox::FindItem_Implementation(const float SearchTime)
{
	TimeToSearch += SearchTime;
	ProgressWidget = ProgressWidget == nullptr ?
					 Cast<UItemLootingProgressWidget>(ProgressWidgetComponent->GetUserWidgetObject()) :
					 ProgressWidget;
	if(ProgressWidget)
	{
		ProgressWidget->SetProgressPercent(TimeToSearch / MaxTimeToSearch);
	}
	if(TimeToSearch >= MaxTimeToSearch)
	{
		LootingItemComponent->GenerateItemsToWorld();
		OpenLooting();
	}
}

void ALootingBox::OpenLooting()
{
	ProgressWidgetComponent->SetVisibility(false);
	if(DissolveMaterialInstance == nullptr)	return;

	DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
	BoxMesh->SetMaterial(0, DynamicDissolveMaterialInstance);
	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Disslove"), -0.55f);
	StartDissolve();
}

void ALootingBox::LootAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(const auto InteractableCharacter = Cast<IInteractableCharacter>(OtherActor))
	{
		ProgressWidgetComponent->SetVisibility(true);
		InteractableCharacter->SetInteractingActor(this);
	}
}

void ALootingBox::LootAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(const auto InteractableCharacter = Cast<IInteractableCharacter>(OtherActor))
	{
		ProgressWidgetComponent->SetVisibility(false);
		InteractableCharacter->SetInteractingActor(nullptr);
	}
}

void ALootingBox::UpdateMaterialDissolve(float DissolveTime)
{
	if(DynamicDissolveMaterialInstance == nullptr)	return;
	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveTime);
}

void ALootingBox::StartDissolve()
{
	if(BoxMesh == nullptr || DissolveCurve == nullptr || DissolveTimeline == nullptr)	return;

	BoxMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	FOnTimelineFloat DissolveTrack;
	DissolveTrack.BindDynamic(this, &ALootingBox::UpdateMaterialDissolve);
	DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);

	FOnTimelineEvent TimelineEndEvent;
	TimelineEndEvent.BindDynamic(this, &ALootingBox::AfterDissolve);
	DissolveTimeline->AddEvent(DissolveTimeline->GetTimelineLength() + 0.1f, TimelineEndEvent);
	DissolveTimeline->Play();
}

void ALootingBox::AfterDissolve()
{
	Destroy();
}