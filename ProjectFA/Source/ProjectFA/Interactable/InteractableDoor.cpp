// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectFA/FAInterfaces/InteractableCharacter.h"
#include "ProjectFA/FAInterfaces/Controller/ItemRPCableController.h"
#include "ProjectFA/HUD/InteractionWidget/ItemLootingProgressWidget.h"

AInteractableDoor::AInteractableDoor()
	:
	DoorMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"))),
	OverlapBoxComponent(CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Box"))),
	ProgressWidgetComponent(CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidget"))),
	DoorOpenTimeline(CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorOpenTimeline"))),
	TimeToOpen(1.f)
{
	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(OverlapBoxComponent);
	DoorMesh->SetupAttachment(GetRootComponent());
	ProgressWidgetComponent->SetupAttachment(GetRootComponent());

	DoorMesh->SetCollisionResponseToAllChannels(ECR_Block);
	DoorMesh->SetRenderCustomDepth(true);

	OverlapBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	OverlapBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapBoxComponent->SetGenerateOverlapEvents(true);
}

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

	ProgressWidgetComponent->SetVisibility(false);
	ProgressWidget = Cast<UItemLootingProgressWidget>(ProgressWidgetComponent->GetUserWidgetObject());
	
	OverlapBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableDoor::InteractBoxBeginOverlap);
	OverlapBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractableDoor::InteractBoxEndOverlap);

	SetupDoorOpenTimeline();
}

void AInteractableDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableDoor, bDoorOpen);
	DOREPLIFETIME(AInteractableDoor, bNowInTimeline);
}

void AInteractableDoor::SetDoorAngle(float Angle)
{
	const FRotator Rotation{ 0, Angle, 0};
	DoorMesh->SetRelativeRotation(Rotation);
}

void AInteractableDoor::InteractBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

void AInteractableDoor::InteractBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

void AInteractableDoor::InteractWithObject_Implementation(const float InteractTime)
{
	if(bNowInTimeline)
	{
		ProgressWidgetComponent->SetVisibility(false);
		return;
	}
	ProgressWidget = ProgressWidget == nullptr ?
				 Cast<UItemLootingProgressWidget>(ProgressWidgetComponent->GetUserWidgetObject()) :
				 ProgressWidget;
	if(ProgressWidget)
	{
		ProgressWidget->SetProgressPercent(InteractTime / TimeToOpen);
	}
	if(InteractTime >= TimeToOpen)
	{
		const auto PlayerController = GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
		if(const auto Controller = Cast<IItemRPCableController>(PlayerController))
		{
			Controller->InteractingWithObject(this);
		}
	}
}

void AInteractableDoor::EndInteracting()
{
	if(bNowInTimeline)	return;
	
	bDoorOpen = !bDoorOpen;
	bNowInTimeline = true;
	OnRep_DoorOpenTriggered();
}

void AInteractableDoor::OnRep_DoorOpenTriggered()
{
	(bDoorOpen) ? DoorOpenTimeline->Play() : DoorOpenTimeline->Reverse();
}

void AInteractableDoor::AfterOpenDoor()
{
	bNowInTimeline = false;
}

void AInteractableDoor::SetupDoorOpenTimeline()
{
	if(DoorOpenTimeline == nullptr || DoorOpenCurve == nullptr)	return;
	
	FOnTimelineFloat DoorOpenTrace;
	DoorOpenTrace.BindDynamic(this, &AInteractableDoor::SetDoorAngle);
	DoorOpenTimeline->AddInterpFloat(DoorOpenCurve, DoorOpenTrace);

	FOnTimelineEvent TimeLineEndEvent;
	TimeLineEndEvent.BindDynamic(this, &AInteractableDoor::AfterOpenDoor);
	DoorOpenTimeline->SetTimelineFinishedFunc(TimeLineEndEvent);
}