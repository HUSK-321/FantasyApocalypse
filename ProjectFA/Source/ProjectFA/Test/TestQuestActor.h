// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectFA/Interactable/Looting/LootInteractable.h"
#include "TestQuestActor.generated.h"

class UBoxComponent;
class UQuestObject;

UCLASS()
class PROJECTFA_API ATestQuestActor : public AActor, public ILootInteractable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> QuestTriggerBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UQuestObject> TestQuestClass;
	
public:	
	ATestQuestActor();

	virtual void FindItem_Implementation(const float SearchTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void TestBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bGiveQuest;
};
