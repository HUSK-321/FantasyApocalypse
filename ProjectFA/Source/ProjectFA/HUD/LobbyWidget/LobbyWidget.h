// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class PROJECTFA_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> StartButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerCountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby", meta = (AllowPrivateAccess = "true"))
	FString LevelPathToPlayGame;
	
public:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnStartButtonClicked();
	void OnPlayerNumChanged(int32 NumOfPlayer);

	void RemoveWidget();
};
