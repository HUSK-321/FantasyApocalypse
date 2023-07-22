// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingBox.h"
#include "LootingItemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectFA/FAInterfaces/InteractableCharacter.h"
#include "ProjectFA/FAInterfaces/Controller/ItemRPCableController.h"
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
	MaxTimeToSearch(2.f),
	bLootingBoxOpened(false), bLootingBoxDissolving(false)
{
	bReplicates = true;
	
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
}

void ALootingBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALootingBox, bLootingBoxOpened);
}

void ALootingBox::OnRep_LootingBoxOpened()
{
	ProgressWidgetComponent->SetVisibility(false);
	if(LootingItemComponent == nullptr)	return;
	
	LootingItemComponent->GenerateItemsToWorld();
	StartDissolve();
}

void ALootingBox::SetSpawnItemList(const TArray<APickupItem*>& ItemList)
{
	LootingItemComponent->InitializeItemList(ItemList);
}

void ALootingBox::InteractWithObject_Implementation(const float SearchTime)
{
	ProgressWidget = ProgressWidget == nullptr ?
					 Cast<UItemLootingProgressWidget>(ProgressWidgetComponent->GetUserWidgetObject()) :
					 ProgressWidget;
	if(ProgressWidget)
	{
		ProgressWidget->SetProgressPercent(SearchTime / MaxTimeToSearch);
	}
	if(SearchTime >= MaxTimeToSearch)
	{
		const auto PlayerController = GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
		if(const auto Controller = Cast<IItemRPCableController>(PlayerController))
		{
			Controller->InteractingWithObject(this);
		}
	}
}

void ALootingBox::EndInteracting()
{
	bLootingBoxOpened = true;
	OnRep_LootingBoxOpened();
}

void ALootingBox::LootAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto OtherPawn = Cast<APawn>(OtherActor);
	if(OtherPawn == nullptr || OtherPawn->IsLocallyControlled() == false)	return;
	if(const auto InteractableCharacter = Cast<IInteractableCharacter>(OtherActor))
	{
		ProgressWidgetComponent->SetVisibility(true);
		InteractableCharacter->SetInteractingActor(this);
	}
}

void ALootingBox::LootAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto OtherPawn = Cast<APawn>(OtherActor);
	if(OtherPawn == nullptr || OtherPawn->IsLocallyControlled() == false)	return;
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
	if(DissolveMaterialInstance == nullptr || bLootingBoxDissolving)	return;
	if(BoxMesh == nullptr || DissolveCurve == nullptr || DissolveTimeline == nullptr)	return;

	bLootingBoxDissolving = true;
	DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Disslove"), -0.55f);
	BoxMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxMesh->SetMaterial(0, DynamicDissolveMaterialInstance);
	BoxMesh->SetRenderCustomDepth(true);
	
	FOnTimelineFloat DissolveTrack;
	DissolveTrack.BindDynamic(this, &ALootingBox::UpdateMaterialDissolve);
	DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);

	FOnTimelineEvent TimelineEndEvent;
	TimelineEndEvent.BindDynamic(this, &ALootingBox::AfterDissolve);
	DissolveTimeline->SetTimelineFinishedFunc(TimelineEndEvent);
	DissolveTimeline->Play();
}

void ALootingBox::AfterDissolve()
{
	Destroy();
}

TArray<FSpawnerInitializeInfo> ALootingBox::GetSpawnCategoryPercent()
{
	return SpawnCategoryInfo;
}