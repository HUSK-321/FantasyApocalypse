// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FAInterfaces/QuestObservable.h"
#include "FAInterfaces/SoundPlayableInstance.h"
#include "FAGameInstance.generated.h"

class UPlayerQuestObserver;

UCLASS()
class PROJECTFA_API UFAGameInstance : public UGameInstance, public IQuestObservable, public ISoundPlayableInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Quest")
	TSubclassOf<UPlayerQuestObserver> PlayerQuestManagementClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Quest")
	TObjectPtr<UPlayerQuestObserver> PlayerQuestManagement;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> GrassFootstep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> MetalFootstep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> WoodFootstep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> WaterFootstep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Sound/Footstep", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> StoneFootstep;

public:
	UFAGameInstance();
	virtual void Init() override;

	virtual void AddEnemyDestroyQuest_Implementation(UQuestObject* Quest) override;
	UFUNCTION()
	void SearchDestroyEnemyQuest(UObject* Enemy);

	virtual void PlayFootstepSoundPhysics(FVector SoundLocation, TEnumAsByte<EPhysicalSurface> SurfaceType, float VolumeMultiplier) override;
};