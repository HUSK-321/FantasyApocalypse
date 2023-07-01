// Fill out your copyright notice in the Description page of Project Settings.


#include "FACharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AFACharacter::AFACharacter()
	:
	MaxHealth(100.f), CurrentHealth(100.f),
	DissolveTimeline(CreateDefaultSubobject<UTimelineComponent>(TEXT("Dissolve Timeline"))),
	bNowInDeadProcess(false)
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AFACharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFACharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFACharacter, CurrentHealth);
}

void AFACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFACharacter::PlayNormalAttackMontage(FName NormalAttackSectionName)
{
	if(NormalAttackMontage == nullptr)	return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance == nullptr)	return;
	AnimInstance->Montage_Play(NormalAttackMontage);
	AnimInstance->Montage_JumpToSection(NormalAttackSectionName);
}

void AFACharacter::StopNormalAttackMontage()
{
	if(NormalAttackMontage == nullptr)	return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance == nullptr)	return;
	AnimInstance->Montage_Stop(0.f, NormalAttackMontage);
}

void AFACharacter::MulticastPlayHitEffect_Implementation(FVector EffectLocation)
{
	if(HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, EffectLocation, FRotator::ZeroRotator);
	}
	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, EffectLocation, FRotator::ZeroRotator);
	}
}

void AFACharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                 AController* InstigatorController, AActor* DamageCauser)
{
}

void AFACharacter::CharacterDead()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance == nullptr)	return;
	
	bNowInDeadProcess = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	GenerateInventoryItems();
	StartDeadDissolve();
}

bool AFACharacter::CharacterCannotMove()
{
	return false;
}

bool AFACharacter::CharacterCannotAttack()
{
	return false;
}

bool AFACharacter::CharacterCannotJump()
{
	return false;
}

void AFACharacter::StartDeadDissolve()
{
	if(GetMesh() == nullptr || DissolveCurve == nullptr || DissolveTimeline == nullptr || DissolveMaterialInstance == nullptr)	return;
	
	GetMesh()->bPauseAnims = true;
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
	const int32 MaterialCount = GetMesh()->GetNumOverrideMaterials();
	for(int i = 0; i < MaterialCount; i++)
	{
		GetMesh()->SetMaterial(i, DynamicDissolveMaterialInstance);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Disslove"), -0.55f);
	}
	
	FOnTimelineFloat DissolveTrack;
	DissolveTrack.BindDynamic(this, &AFACharacter::UpdateMaterialDissolve);
	DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);

	FOnTimelineEvent TimelineEndEvent;
	TimelineEndEvent.BindDynamic(this, &AFACharacter::AfterDeadDissolve);
	DissolveTimeline->SetTimelineFinishedFunc(TimelineEndEvent);
	DissolveTimeline->Play();
}

void AFACharacter::CurrentHealthChanged()
{}

void AFACharacter::UpdateMaterialDissolve(float DissolveTime)
{
	if(DynamicDissolveMaterialInstance == nullptr)	return;
	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveTime);
}

void AFACharacter::AfterDeadDissolve()
{
	Destroy();
}

void AFACharacter::OnRep_CurrentHealthChanged()
{
	CurrentHealthChanged();
}