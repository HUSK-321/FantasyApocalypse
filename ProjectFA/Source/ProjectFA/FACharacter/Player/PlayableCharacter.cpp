#include "PlayableCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InventoryComponent.h"
#include "PlayableCharacterCombatComponent.h"
#include "PlayableController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectFA/HUD/InventoryWidget/InventorySlotWidget.h"
#include "ProjectFA/Interactable/Looting/LootInteractable.h"

APlayableCharacter::APlayableCharacter()
	:
	CameraSpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"))),
	FollowCamera(CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"))),
	CombatComponent(CreateDefaultSubobject<UPlayableCharacterCombatComponent>(TEXT("CombatComponent"))),
	InventoryComponent(CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"))),
	MaxWalkSpeed(400.f), MaxSprintSpeed(700.f), MaxCrouchSpeed(300.f), bNowSprinting(false),
	MaxStamina(100.f), CurrentStamina(100.f),
	StaminaIncreaseFactor(10.f), StaminaDecreaseFactor(20.f), JumpStaminaConsume(20.f),
	InventoryWeightFactor(0.f),
	InteractingTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm->SetupAttachment(GetMesh());
	CameraSpringArm->TargetArmLength = 600.f;
	CameraSpringArm->bUsePawnControlRotation = true;
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 100.0f), FRotator::ZeroRotator);

	FollowCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	CombatComponent->SetIsReplicated(true);
}

void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterMoveSpeed();

	OnTakeAnyDamage.AddDynamic(this, &APlayableCharacter::ReceiveDamage);
	InventoryComponent->InventoryWeightChangedEvent.AddDynamic(this, &APlayableCharacter::SetInventoryWeightSpeedFactor);

	if(const auto PlayableController = Cast<APlayableController>(GetController()))
	{
		PlayableController->SetPlayerEvent(this);
		PlayableController->SetInventoryEvent(InventoryComponent);
		if(const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayableController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerBasicMappingContext, 0);
		}
	}
	PlayerHealthChangedEvent.Broadcast(CurrentHealth, MaxHealth);
	PlayerStaminaChangedEvent.Broadcast(CurrentStamina, MaxStamina);
}

void APlayableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APlayableCharacter::CharacterDead()
{
	// TODO : call AfterDeath in anim montage, not here
	Super::CharacterDead();

	AfterDeath();
}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayableCharacter::CharacterMove);
		EnhancedInputComponent->BindAction(CameraAction, ETriggerEvent::Triggered, this, &APlayableCharacter::CameraMove);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayableCharacter::Jump);
		EnhancedInputComponent->BindAction(InteractNearbyItem, ETriggerEvent::Started, this, &APlayableCharacter::InteractWithNearbyItem);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayableCharacter::AttackButtonPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayableCharacter::CrouchButtonPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayableCharacter::SprintButtonPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayableCharacter::SprintButtonReleased);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &APlayableCharacter::ScrollNearbyItemList);

		EnhancedInputComponent->BindAction(InteractWithObject, ETriggerEvent::Triggered, this, &APlayableCharacter::InteractWithActors);
		EnhancedInputComponent->BindAction(InteractWithObject, ETriggerEvent::Completed, this, &APlayableCharacter::InteractWithActorsEnd);
	}
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ThisClass::InventoryButtonPressed);
}

void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ManageStaminaAmount(DeltaTime);
}

void APlayableCharacter::CharacterMove(const FInputActionValue& Value)
{
	if(GetController() == nullptr || CharacterCannotMove())	return;
	
	const auto MoveVector = Value.Get<FVector2D>();
	const FRotator YawRotation{ 0.f, Controller->GetControlRotation().Yaw, 0.f };
	const FVector ForwardDirection{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
	const FVector RightDirection{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void APlayableCharacter::CameraMove(const FInputActionValue& Value)
{
	if(GetController() == nullptr)	return;
	
	const auto LookAxisValue = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void APlayableCharacter::Jump()
{
	if(CharacterCannotJump())	return;
	if(bIsCrouched)
	{
		UnCrouch();
		return;
	}
	Super::Jump();
}

void APlayableCharacter::CrouchButtonPressed()
{
	if(bNowSprinting || CharacterCannotMove())	return;
	bIsCrouched ? UnCrouch() : Crouch();
}

void APlayableCharacter::SprintButtonPressed()
{
	if(CharacterCannotMove())	return;
	if(bIsCrouched)
	{
		UnCrouch();
	}
	SetSprinting(true);
}

void APlayableCharacter::SprintButtonReleased()
{
	SetSprinting(false);
}

void APlayableCharacter::SetSprinting(bool bSprinting)
{
	bNowSprinting = bSprinting;
	SetCharacterMoveSpeed();
}

void APlayableCharacter::ManageStaminaAmount(float DeltaTime)
{
	CurrentStamina += (bNowSprinting) ? -DeltaTime * StaminaDecreaseFactor : DeltaTime * StaminaIncreaseFactor;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
	if(CurrentStamina <= 0.f)
	{
		SetSprinting(false);
	}
	PlayerStaminaChangedEvent.Broadcast(CurrentStamina, MaxStamina);
}

void APlayableCharacter::SetCharacterMoveSpeed()
{
	if(GetCharacterMovement() == nullptr)	return;
	
	const float DecreaseFactors = InventoryWeightFactor;
	const float CurrentSpeed = (bNowSprinting) ? MaxSprintSpeed - DecreaseFactors :
												 MaxWalkSpeed - DecreaseFactors;
	const float CurrentCrouchSpeed = MaxCrouchSpeed - DecreaseFactors;
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentCrouchSpeed;

	ServerSetCharacterMovement(bNowSprinting);
}

void APlayableCharacter::SetInventoryWeightSpeedFactor(const float& InventoryTotalWeight)
{
	InventoryWeightFactor = InventoryTotalWeight;
	SetSprinting(bNowSprinting);
}

void APlayableCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	PlayerHealthChangedEvent.Broadcast(CurrentHealth, MaxHealth);
	if(CurrentHealth <= 0.f)
	{
		CharacterDead();
	}
}

void APlayableCharacter::SetNearbyItem(AActor* PickupItem)
{
	if(InventoryComponent == nullptr)	return;
	InventoryComponent->AddNearbyItem(PickupItem);
}

void APlayableCharacter::UnsetNearbyItem(AActor* PickupItem)
{
	if(InventoryComponent == nullptr)	return;
	InventoryComponent->DeleteNearbyItem(PickupItem);
}

UActorComponent* APlayableCharacter::GetCombatComponent() const
{
	return CombatComponent;
}

void APlayableCharacter::SetInteractingActor(AActor* Actor)
{
	InteractingTime = 0.f;
	InteractingActor = Actor;
	if(InteractingActor == nullptr)
	{
		SetInteractMappingContext(false);
		return;
	}
	SetInteractMappingContext(true);
}

void APlayableCharacter::InteractWithNearbyItem()
{
	if(InventoryComponent == nullptr)	return;
	InventoryComponent->SetNearbyItemToInventory();
}

void APlayableCharacter::InteractWithActors(const FInputActionValue& Value)
{
	if(InteractingActor == nullptr)	return;
	if(UKismetSystemLibrary::DoesImplementInterface(InteractingActor, ULootInteractable::StaticClass()))
	{
		InteractingTime += GetWorld()->GetDeltaSeconds();
		ILootInteractable::Execute_FindItem(InteractingActor, InteractingTime);
	}
}

void APlayableCharacter::InteractWithActorsEnd()
{
	InteractingTime = 0.f;
}

void APlayableCharacter::AttackButtonPressed()
{
	if(CombatComponent == nullptr || CharacterCannotAttack())	return;
	CombatComponent->Attack();
}

void APlayableCharacter::ScrollNearbyItemList(const FInputActionValue& Value)
{
	if(GetController() == nullptr || InventoryComponent == nullptr)	return;
	
	auto ScrollAmount = Value.Get<float>();
	ScrollAmount = FMath::Clamp(ScrollAmount, -1.0f, 1.0f);
	InventoryComponent->ScrollNearbyItemList(ScrollAmount);
}

void APlayableCharacter::InventoryButtonPressed()
{
	if(const auto PlayableController = Cast<APlayableController>(Controller))
	{
		PlayableController->ToggleInventoryWidget();
	}
}

void APlayableCharacter::SetInteractMappingContext(bool bIsActive)
{
	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bIgnoreAllPressedKeysUntilRelease = false;
	if(const auto PlayableController = Cast<APlayableController>(GetController()))
	{
		if(const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayableController->GetLocalPlayer()))
		{
			bIsActive ? Subsystem->AddMappingContext(InteractMappingContext, 1, ModifyContextOptions) :
						Subsystem->RemoveMappingContext(InteractMappingContext, ModifyContextOptions);
		}
	}
}

void APlayableCharacter::AfterDeath()
{
	InventoryComponent->GenerateItemsToWorld();
	StartDeadDissolve();
}

bool APlayableCharacter::CharacterCannotMove()
{
	return false;
}

bool APlayableCharacter::CharacterCannotAttack()
{
	return bIsCrouched || GetCharacterMovement()->IsFalling();
}

bool APlayableCharacter::CharacterCannotJump()
{
	if(CombatComponent == nullptr)	return CharacterCannotMove();
	return CharacterCannotMove() || CombatComponent->GetNowAttacking();
}

void APlayableCharacter::ServerSetCharacterMovement_Implementation(bool bSprinting)
{
	if(GetCharacterMovement() == nullptr)	return;
	
	const float DecreaseFactors = InventoryWeightFactor;
	const float CurrentSpeed = (bSprinting) ? MaxSprintSpeed - DecreaseFactors :
												 MaxWalkSpeed - DecreaseFactors;
	const float CurrentCrouchSpeed = MaxCrouchSpeed - DecreaseFactors;
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentCrouchSpeed;
}