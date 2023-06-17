// Fill out your copyright notice in the Description page of Project Settings.


#include "TestQuestActor.h"
#include "Components/BoxComponent.h"
#include "ProjectFA/FACharacter/InteractableCharacter.h"
#include "ProjectFA/FAInterfaces/QuestObservable.h"
#include "ProjectFA/FAQuests/QuestObject.h"

ATestQuestActor::ATestQuestActor()
	:
	bGiveQuest(false)
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	QuestTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Test Box Component"));
	QuestTriggerBox->SetupAttachment(GetRootComponent());
}

void ATestQuestActor::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		if(QuestTriggerBox)
		{
			QuestTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATestQuestActor::TestBeginOverlap);
		}
	}
}

void ATestQuestActor::FindItem_Implementation(const float SearchTime)
{
	if(SearchTime >= 0.1f && bGiveQuest == false && TestQuestClass)
	{
		const auto FAGameInstance = Cast<IQuestObservable>(GetGameInstance());
		if(FAGameInstance == nullptr)	return;

		const auto NewQuest = NewObject<UQuestObject>(GetWorld(), TestQuestClass);
		FAGameInstance->Execute_AddEnemyDestroyQuest(GetGameInstance(), NewQuest);
		bGiveQuest = true;
	}
}

void ATestQuestActor::TestBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto OtherPawn = Cast<APawn>(OtherActor);
	if(OtherPawn == nullptr || OtherPawn->IsLocallyControlled() == false)	return;
	if(const auto InteractableCharacter = Cast<IInteractableCharacter>(OtherActor))
	{
		InteractableCharacter->SetInteractingActor(this);
	}
}
