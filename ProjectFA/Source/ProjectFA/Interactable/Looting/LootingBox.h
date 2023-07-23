// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectFA/FAInterfaces/InteractableWithCharacter.h"
#include "ProjectFA/PlayGamePretreatment/ItemSpawnable.h"
#include "LootingBox.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ULootingItemComponent;
class UWidgetComponent;
class UItemLootingProgressWidget;

UCLASS()
class PROJECTFA_API ALootingBox : public AActor, public IInteractableWithCharacter, public IItemSpawnable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BoxMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> BoxArea;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> ItemGeneratePosition;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULootingItemComponent> LootingItemComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> ProgressWidgetComponent;
	UPROPERTY()
	TObjectPtr<UItemLootingProgressWidget> ProgressWidget;

	/** Runtime changeable dynamic instance */
	UPROPERTY(VisibleAnywhere, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;
	/** Material instance set on the bp, use with dynamic material instance */
	UPROPERTY(EditAnywhere, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(VisibleAnywhere, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> DissolveTimeline;
	UPROPERTY(EditAnywhere, Category = "Dissolve")
	TObjectPtr<UCurveFloat> DissolveCurve;

	UPROPERTY(EditAnywhere, Category = "Looting Property")
	float MaxTimeToSearch;
	UPROPERTY(EditAnywhere, Category = "Spawner Property")
	TArray<FSpawnerInitializeInfo> SpawnCategoryInfo;

	UPROPERTY(ReplicatedUsing = OnRep_LootingBoxOpened)
	bool bLootingBoxOpened;
	bool bLootingBoxDissolving;

public:	
	ALootingBox();

	virtual void InteractWithObject_Implementation(const float SearchTime) override;
	virtual void EndInteracting() override;	

	virtual TArray<FSpawnerInitializeInfo> GetSpawnCategoryPercent() override;
	virtual void SetSpawnItemList(const TArray<APickupItem*>& ItemList) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	void StartDissolve();
	UFUNCTION()
	void LootAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void LootAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void UpdateMaterialDissolve(float DissolveTime);
	UFUNCTION()
	void AfterDissolve();
	
	UFUNCTION()
	void OnRep_LootingBoxOpened();
};