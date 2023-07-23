// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableController.h"
#include "InventoryComponent.h"
#include "PlayableCharacter.h"
#include "PlayableCharacterCombatComponent.h"
#include "Components/ListView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ProjectFA/HUD/PlayerOverlay.h"
#include "ProjectFA/HUD/SkillWidget/SkillWidget.h"
#include "ProjectFA/HUD/ProjectFAHUD.h"
#include "ProjectFA/HUD/InventoryWidget/InventoryWidget.h"
#include "ProjectFA/HUD/PickupItemListWidget/PickupItemList.h"
#include "GameFramework/PlayerState.h"
#include "ProjectFA/FAInterfaces/InteractableWithCharacter.h"
#include "ProjectFA/FAInterfaces/InventoryUsable.h"
#include "ProjectFA/HUD/Handslot/PlayerHandSlotWidget.h"

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
	if(PlayerHealthOverlayNotValid())	return;
	const auto& Overlay = ProjectFAHUD->PlayerOverlay;
	const float HealthPercent = CurrentHealth / MaxHealth;
	const FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(CurrentHealth), FMath::CeilToInt(MaxHealth));
	Overlay->HealthBar->SetPercent(HealthPercent);
	Overlay->HealthText->SetText(FText::FromString(HealthText));
}

void APlayableController::SetStaminaHUD(const float& CurrentStamina, const float& MaxStamina)
{
	if(PlayerStaminaOverlayNotValid())	return;
	const float StaminaPercent = CurrentStamina / MaxStamina;
	ProjectFAHUD->PlayerOverlay->StaminaBar->SetPercent(StaminaPercent);
}

void APlayableController::AddNearbyItem(UObject* Item)
{
	if(NearbyItemListNotValid())	return;
	ProjectFAHUD->PickupItemList->NearbyItemList->AddItem(Item);
	ProjectFAHUD->PickupItemList->SetVisibility(ESlateVisibility::Visible);
}

void APlayableController::DeleteNearbyItem(UObject* Item)
{
	if(NearbyItemListNotValid())	return;
	ProjectFAHUD->PickupItemList->NearbyItemList->RemoveItem(Item);
	if(ProjectFAHUD->PickupItemList->NearbyItemList->GetNumItems() == 0)
	{
		ProjectFAHUD->PickupItemList->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayableController::ScrollNearbyItemList(int32 ScrollIndex)
{
	if(NearbyItemListNotValid())	return;
	ProjectFAHUD->PickupItemList->SetScrollIndex(ScrollIndex);
}

void APlayableController::AddInventoryItem(const TArray<APickupItem*> ItemList)
{
	if(InventoryWidgetNotValid())	return;
	ProjectFAHUD->Inventory->SetInventoryWidgetList(ItemList);
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
	if(SkillWidgetNotValid())	return;
	ProjectFAHUD->PlayerOverlay->SkillWidget->SetSkillSlotWidget(QSkillData, ESkillData);
}

void APlayableController::CurrentHandItemWidget(APickupItem* ItemInHand)
{
	if(ItemInHand == nullptr || ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->HandSlotWidget == nullptr)	return;
	
	ProjectFAHUD->PlayerOverlay->HandSlotWidget->SetWeaponImage(ItemInHand->GetItemIcon());
}

void APlayableController::AnnouncePlayer(FString AnnounceText)
{
	ClientAnnouncePlayer(AnnounceText);
}

void APlayableController::ClientAnnouncePlayer_Implementation(const FString& AnnounceText)
{
	if(ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr)	return;
	ProjectFAHUD->PlayerOverlay->SetAnnounceText(AnnounceText);
}

void APlayableController::DisableAnnounce()
{
	ClientDisableAnnounce();
}

void APlayableController::ClientDisableAnnounce_Implementation()
{
	if(ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr)	return;
	ProjectFAHUD->PlayerOverlay->DisableAnnounce();
}

void APlayableController::SetInventoryWeight(const float& Weight)
{
	if(InventoryWidgetNotValid() || ProjectFAHUD->Inventory->TotalWeight == nullptr)	return;
	const FString WeightTextString = FString::Printf(TEXT("Total Weight : %.2fKG"), Weight); 
	ProjectFAHUD->Inventory->TotalWeight->SetText(FText::FromString(WeightTextString));
}

bool APlayableController::PlayerHealthOverlayNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->HealthText == nullptr || ProjectFAHUD->PlayerOverlay->HealthBar == nullptr;
}

bool APlayableController::NearbyItemListNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->PickupItemList == nullptr || ProjectFAHUD->PickupItemList->NearbyItemList == nullptr;
}

bool APlayableController::PlayerStaminaOverlayNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->StaminaBar == nullptr;
}

bool APlayableController::InventoryWidgetNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->Inventory == nullptr;
}

bool APlayableController::SkillWidgetNotValid() const
{
	return ProjectFAHUD == nullptr || ProjectFAHUD->PlayerOverlay == nullptr || ProjectFAHUD->PlayerOverlay->SkillWidget == nullptr;
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