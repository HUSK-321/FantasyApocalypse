#include "PlayableCharacter.h"

#include "InventoryComponent.h"
#include "PlayableCharacterCombatComponent.h"
#include "PlayableController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayableCharacter::APlayableCharacter()
	:
	CameraSpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"))),
	FollowCamera(CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"))),
	CombatComponent(CreateDefaultSubobject<UPlayableCharacterCombatComponent>(TEXT("CombatComponent"))),
	InventoryComponent(CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"))),
	MaxWalkSpeed(400.f), MaxSprintSpeed(700.f), bNowSprinting(false),
	MaxStamina(100.f), CurrentStamina(100.f),
	StaminaIncreaseFactor(10.f), StaminaDecreaseFactor(20.f)
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
}

void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	OnTakeAnyDamage.AddDynamic(this, &APlayableCharacter::ReceiveDamage);

	if(auto PlayableController = Cast<APlayableController>(Controller))
	{
		// TODO : Controller should do this, not in player
		PlayableController->SetPlayerEvent(this);
	}
	PlayerHealthChangedEvent.Broadcast(CurrentHealth, MaxHealth);
	PlayerStaminaChangedEvent.Broadcast(CurrentStamina, MaxStamina);
}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ThisClass::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ThisClass::SprintButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ThisClass::SprintButtonReleased);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ThisClass::InteractionButtonPressed);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ThisClass::AttackButtonPressed);

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
	PlayerInputComponent->BindAxis("TurnRight", this, &ThisClass::TurnRight);
}

void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ManageStaminaAmount(DeltaTime);
}

void APlayableCharacter::MoveForward(float Value)
{
	if(Controller == nullptr || Value == 0.f)	return;

	const FRotator YawRotation{ 0.f, Controller->GetControlRotation().Yaw, 0.f };
	const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
	AddMovementInput(Direction, Value);
}

void APlayableCharacter::MoveRight(float Value)
{
	if(Controller == nullptr || Value == 0.f)	return;

	const FRotator YawRotation{ 0.f, Controller->GetControlRotation().Yaw, 0.f };
	const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
	AddMovementInput(Direction, Value);
}

void APlayableCharacter::TurnRight(float Value)
{
	if(Controller == nullptr || Value == 0.f)	return;

	AddControllerYawInput(Value);
}

void APlayableCharacter::LookUp(float Value)
{
	if(Controller == nullptr || Value == 0.f)	return;

	AddControllerPitchInput(Value);
}

void APlayableCharacter::Jump()
{
	if(CurrentStamina < JumpStaminaConsume)	return;
	if(bIsCrouched)
	{
		UnCrouch();
		return;
	}
	CurrentStamina = FMath::Clamp(CurrentStamina - JumpStaminaConsume, 0.f, MaxStamina);
	Super::Jump();
}

void APlayableCharacter::CrouchButtonPressed()
{
	if(bNowSprinting)	return;
	if(bIsCrouched)
	{
		UnCrouch();
		return;
	}
	Crouch();
}

void APlayableCharacter::SprintButtonPressed()
{
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
	GetCharacterMovement()->MaxWalkSpeed = bNowSprinting ? MaxSprintSpeed : MaxWalkSpeed;
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

void APlayableCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	PlayerHealthChangedEvent.Broadcast(CurrentHealth, MaxHealth);
}

void APlayableCharacter::SetCurrentPickupItem(APickupItem* PickupItem)
{
	CurrentlyNearItem = PickupItem;
}

void APlayableCharacter::InteractionButtonPressed()
{
	if(CurrentlyNearItem == nullptr)	return;
	InventoryComponent->GetItemToInventory(CurrentlyNearItem);	
}

void APlayableCharacter::AttackButtonPressed()
{
	if(CombatComponent == nullptr)	return;
	CombatComponent->Attack();
}