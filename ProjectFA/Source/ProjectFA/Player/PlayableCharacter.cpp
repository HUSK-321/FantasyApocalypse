#include "PlayableCharacter.h"

#include "PlayableController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayableCharacter::APlayableCharacter()
	:
	CameraSpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"))),
	FollowCamera(CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"))),
	MaxWalkSpeed(400.f), MaxSprintSpeed(700.f), bNowSprinting(false),
	MaxHealth(100.f), CurrentHealth(100.f), MaxStamina(100.f), CurrentStamina(100.f),
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

	SetHealthHUD();
	SetStaminaHUD();
}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ThisClass::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ThisClass::SprintButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ThisClass::SprintButtonReleased);

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
	if(bIsCrouched)
	{
		UnCrouch();
		return;
	}
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

	SetStaminaHUD();
}

void APlayableCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	SetHealthHUD();
}

void APlayableCharacter::SetHealthHUD()
{
	PlayableController = (PlayableController == nullptr) ? Cast<APlayableController>(Controller) : PlayableController;
	if(PlayableController == nullptr)	return;
	PlayableController->SetHealthHUD(CurrentHealth, MaxHealth);
}

void APlayableCharacter::SetStaminaHUD()
{
	PlayableController = (PlayableController == nullptr) ? Cast<APlayableController>(Controller) : PlayableController;
	if(PlayableController == nullptr)	return;
	PlayableController->SetStaminaHUD(CurrentStamina, MaxStamina);
}