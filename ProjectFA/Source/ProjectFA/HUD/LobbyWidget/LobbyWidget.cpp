// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProjectFA/FAGameModes/LobbyGameMode.h"

bool ULobbyWidget::Initialize()
{
	if(Super::Initialize() == false || StartButton == nullptr || PlayerCountText == nullptr || GetGameInstance() == nullptr)	return false;

	if(GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority() == false)
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		return true;
	}
	
	StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnStartButtonClicked);
	if(auto World = GetWorld())
	{
		if(auto LobbyGameMode = Cast<ALobbyGameMode>(World->GetAuthGameMode()))
		{
			LobbyGameMode->OnNewPlayerJoined.AddUObject(this, &ULobbyWidget::OnPlayerNumChanged);
			OnPlayerNumChanged(LobbyGameMode->GetNumPlayers());
		}

		if(const auto PlayerController = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetWidgetToFocus(TakeWidget());
			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	return true;
}

void ULobbyWidget::OnStartButtonClicked()
{
	if(UWorld* World = GetWorld())
	{
		const FString LevelToTravel = FString::Printf(TEXT("%s?listen"), *LevelPathToPlayGame);
		World->ServerTravel(LevelToTravel);

		RemoveWidget();
	}
}

void ULobbyWidget::OnPlayerNumChanged(int32 NumOfPlayer)
{
	if(PlayerCountText == nullptr)	return;
	PlayerCountText->SetText(FText::AsNumber(NumOfPlayer));
}

void ULobbyWidget::RemoveWidget()
{
	RemoveFromParent();
	if(const UWorld* World = GetWorld())
	{
		if(APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			const FInputModeGameOnly InputModeGameOnly;
			PlayerController->SetInputMode(InputModeGameOnly);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
