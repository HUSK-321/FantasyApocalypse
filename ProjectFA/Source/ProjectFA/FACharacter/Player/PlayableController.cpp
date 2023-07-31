// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableController.h"
#include "InventoryComponent.h"
#include "PlayableCharacter.h"
#include "PlayableCharacterCombatComponent.h"
#include "ProjectFA/HUD/ProjectFAHUD.h"
#include "ProjectFA/HUD/InventoryWidget/InventoryWidget.h"
#include "GameFramework/PlayerState.h"
#include "ProjectFA/FAInterfaces/InteractableWithCharacter.h"
#include "ProjectFA/FAInterfaces/InventoryUsable.h"

void APlayableController::BeginPlay()
{
	Super::BeginPlay();

	ProjectFAHUD = Cast<AProjectFAHUD>(GetHUD());
}

void APlayableController::OnUnPossess()
{
	Super::OnUnPossess();

	SetPlayerSpectate();
}

void APlayableController::InteractingWithObject(UObject* LootingBox)
{
	ServerInteracting(LootingBox);
}

void APlayableController::DropItem(APickupItem* Item)
{
	ServerDropItem(Item);
}

void APlayableController::UseItem(UObject* Item)
{
	ServerUseItem(Item);
}

void APlayableController::ServerInteracting_Implementation(UObject* LootingBox)
{
	if(IsValid(LootingBox) == false)	return;
	const auto LootableBox = Cast<IInteractableWithCharacter>(LootingBox);
	if(LootableBox == nullptr)	return;
	
	LootableBox->EndInteracting();
}

void APlayableController::ServerDropItem_Implementation(APickupItem* Item)
{
	if(IsValid(Item) == false)	return;
	Item->MulticastDrop();
}

void APlayableController::ServerUseItem_Implementation(UObject* Item)
{
	const auto UsableItem = Cast<IInventoryUsable>(Item);
	if(UsableItem == nullptr)	return;
	UsableItem->UseAction();
}

void APlayableController::SetPlayerEvent(APlayableCharacter* ControllingPlayer)
{
	ControllingPlayer->PlayerHealthChangedEvent.AddDynamic(this, &APlayableController::SetHealthHUD);
	ControllingPlayer->PlayerStaminaChangedEvent.AddDynamic(this, &APlayableController::SetStaminaHUD);
}

void APlayableController::SetInventoryEvent(UInventoryComponent* InventoryComponent)
{
	InventoryComponent->NearbyItemAddEvent.AddDynamic(this, &APlayableController::AddNearbyItem);
	InventoryComponent->NearbyItemDeleteEvent.AddDynamic(this, &APlayableController::DeleteNearbyItem);
	InventoryComponent->InventoryChangedEvent.AddDynamic(this, &APlayableController::AddInventoryItem);
	InventoryComponent->InventoryWeightChangedEvent.AddDynamic(this, &APlayableController::SetInventoryWeight);
	InventoryComponent->NearbyListScrollChangedEvent.AddDynamic(this, &APlayableController::ScrollNearbyItemList);
}

void APlayableController::SetCombatComponentEvent(UPlayableCharacterCombatComponent* CombatComponent)
{
	CombatComponent->OnPlayerHandItemChanged.AddDynamic(this, &APlayableController::CurrentHandItemWidget);
}

void APlayableController::SetHealthHUD(const float& CurrentHealth, const float& MaxHealth)
{
	if(IsValid(ProjectFAHUD) == false)	return;

	ProjectFAHUD->SetHealth(CurrentHealth, MaxHealth);
}

void APlayableController::SetStaminaHUD(const float& CurrentStamina, const float& MaxStamina)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->SetStamina(CurrentStamina, MaxStamina);
}

void APlayableController::AddNearbyItem(UObject* Item)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->AddNearbyItem(Item);
}

void APlayableController::DeleteNearbyItem(UObject* Item)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->DeleteNearbyItem(Item);
}

void APlayableController::ScrollNearbyItemList(int32 ScrollIndex)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->ScrollNearbyItemList(ScrollIndex);
}

void APlayableController::AddInventoryItem(const TArray<APickupItem*> ItemList)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->AddInventoryItem(ItemList);
}

void APlayableController::ToggleInventoryWidget()
{
	if(InventoryWidgetNotValid())	return;
	const auto bIsVisible = ProjectFAHUD->Inventory->GetVisibility() == ESlateVisibility::Visible;
	const auto Visibility = bIsVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	ProjectFAHUD->Inventory->SetVisibility(Visibility);
	Visibility == ESlateVisibility::Visible ? SetInputModeGameAndUI() : SetInputModeGameOnly();
}

void APlayableController::InitializeSkillWidget(USkillDataAsset* QSkillData, USkillDataAsset* ESkillData)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->InitializeSkillWidget(QSkillData, ESkillData);
}

void APlayableController::CurrentHandItemWidget(APickupItem* ItemInHand)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->CurrentHandItemWidget(ItemInHand);
}

void APlayableController::AnnouncePlayer(FString AnnounceText)
{
	ClientAnnouncePlayer(AnnounceText);
}

void APlayableController::ClientAnnouncePlayer_Implementation(const FString& AnnounceText)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->SetAnnounceText(AnnounceText);
}

void APlayableController::DisableAnnounce()
{
	ClientDisableAnnounce();
}

void APlayableController::ClientDisableAnnounce_Implementation()
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->DisableAnnounce();
}

void APlayableController::SetInventoryWeight(const float& Weight)
{
	if(IsValid(ProjectFAHUD) == false)	return;
	
	ProjectFAHUD->SetInventoryWeight(Weight);
}

bool APlayableController::InventoryWidgetNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->Inventory == nullptr;
}

void APlayableController::SetInputModeGameAndUI()
{
	const FInputModeGameAndUI InputModeGameAndUI;
	SetInputMode(InputModeGameAndUI);
	bShowMouseCursor = true;
}

void APlayableController::SetInputModeGameOnly()
{
	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	bShowMouseCursor = false;
}

void APlayableController::SetPlayerSpectate()
{
	if(HasAuthority() == false || PlayerState == nullptr)	return;

	StartSpectatingOnly();
	
	PlayerState->SetIsSpectator(true);
	ChangeState(NAME_Spectating);
	bPlayerIsWaiting = true;

	ViewAPlayer(1);
	ClientGotoState(NAME_Spectating);
	ClientHUDChangedToDead();
}

void APlayableController::ClientViewTarget_Implementation()
{
	if(GetViewTarget())
	{
		OnSpectatorViewTargetChanged.Broadcast(GetViewTarget());
	}
}

void APlayableController::ViewAPlayer(int32 dir)
{
	Super::ViewAPlayer(dir);
	
	ClientViewTarget();
}

void APlayableController::ClientHUDChangedToDead_Implementation()
{
	if(ProjectFAHUD)
	{
		ProjectFAHUD->PlayerSetToDead();
	}
}