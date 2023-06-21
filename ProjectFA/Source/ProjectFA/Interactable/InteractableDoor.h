// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Looting/InteractableWithCharacter.h"
#include "InteractableDoor.generated.h"

class UTimelineComponent;
class UBoxComponent;
class UItemLootingProgressWidget;
class UWidgetComponent;

UCLASS()
class PROJECTFA_API AInteractableDoor : public AActor, public IInteractableWithCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> OverlapBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> ProgressWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UItemLootingProgressWidget> ProgressWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> DoorOpenTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> DoorOpenCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
	float TimeToOpen;
	FTimerHandle DoorOpenTimer;

	UPROPERTY(ReplicatedUsing = OnRep_DoorOpenTriggered)
	bool bDoorOpen;
	UPROPERTY(Replicated)
	bool bNowInTimeline;
	
public:	
	AInteractableDoor();

	virtual void InteractWithObject_Implementation(const float InteractTime) override;
	virtual void EndInteracting() override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void SetupDoorOpenTimeline();
	UFUNCTION()
	void SetDoorAngle(float Angle);
	UFUNCTION()
	void OnRep_DoorOpenTriggered();
	UFUNCTION()
	void AfterOpenDoor();
	UFUNCTION()
	void InteractBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void InteractBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};