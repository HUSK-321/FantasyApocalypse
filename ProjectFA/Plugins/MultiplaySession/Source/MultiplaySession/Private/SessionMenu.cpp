// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionMenu.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

bool USessionMenu::Initialize()
{
	if(Super::Initialize() == false || HostButton == nullptr || JoinButton == nullptr)	return false;
	
	HostButton->OnClicked.AddDynamic(this, &USessionMenu::HostButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &USessionMenu::JoinButtonClicked);
	return true;
}

void USessionMenu::NativeDestruct()
{
	RemoveWidget();
	Super::NativeDestruct();
}

void USessionMenu::SetupMenu(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	const UWorld* World = GetWorld();
	if(World == nullptr)	return;
	const auto PlayerController = World->GetFirstPlayerController();
	if(PlayerController == nullptr)	return;

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->SetShowMouseCursor(true);

	if(const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &USessionMenu::OnCreateSession);
		MultiplayerSessionSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &USessionMenu::OnFindSessions);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &USessionMenu::OnJoinSession);
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &USessionMenu::OnDestroySession);
		MultiplayerSessionSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &USessionMenu::OnStartSession);
	}
}

void USessionMenu::HostButtonClicked()
{
	if(HostButton)
	{
		HostButton->SetIsEnabled(false);
	}
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void USessionMenu::JoinButtonClicked()
{
	if(JoinButton)
	{
		JoinButton->SetIsEnabled(false);
	}
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->FindSessions(10000);
		
	}
}

void USessionMenu::RemoveWidget()
{
	RemoveFromParent();
	if(UWorld* World = GetWorld())
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeGameOnly InputModeGameOnly;
			PlayerController->SetInputMode(InputModeGameOnly);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void USessionMenu::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		if(UWorld* World = GetWorld())
		{
			World->ServerTravel(FString(PathToLobby));
		}
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("session create Successfully [USessionMenu::OnCreateSession]")));
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("session create FAIL [USessionMenu::OnCreateSession]")));
		if(HostButton)
		{
			HostButton->SetIsEnabled(true);
		}
	}
}

void USessionMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if(MultiplayerSessionSubsystem == nullptr)	return;
	for(auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if(SettingsValue != MatchType)	continue;

		MultiplayerSessionSubsystem->JoinSession(Result);
	}

	if(bWasSuccessful)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Find Session Failed [USessionMenu::OnFindSessions]")));
		}
	}
	if(SessionResults.Num() == 0)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Session number is ZERO [USessionMenu::OnFindSessions]")));
		}
	}

	if(bWasSuccessful == false || SessionResults.Num() == 0 && JoinButton)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void USessionMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if(const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		const auto SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			if(APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}

	if(Result != EOnJoinSessionCompleteResult::Success && JoinButton)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("join session not success [USessionMenu::OnJoinSession]")));
		}
		JoinButton->SetIsEnabled(true);
	}
}

void USessionMenu::OnDestroySession(bool bWasSuccessful)
{
}

void USessionMenu::OnStartSession(bool bWasSuccessful)
{
}
