// Copyright Jonwoo-Choi


#include "Character//CavePlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "CaveGameplayTags.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "Player/CavePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CavePlayerController.h"
#include "UI/HUD/CaveHUD.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"

ACavePlayerCharacter::ACavePlayerCharacter()
{
	Tags.Add(FName("Player"));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 750.f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = BaseRotationRate;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseMaxWalkSpeed;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	CharacterClass = ECharacterClass::Player;
}

void ACavePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();

}

void ACavePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

int32 ACavePlayerCharacter::GetCharacterLevel_Implementation() const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->GetPlayerLevel();
}

int32 ACavePlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ACavePlayerCharacter::GetXP_Implementation() const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->GetXP();
}

int32 ACavePlayerCharacter::GetAttributePoints_Implementation() const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->GetAttributePoints();
}

int32 ACavePlayerCharacter::GetSpellPoints_Implementation() const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->GetSpellPoints();
}

int32 ACavePlayerCharacter::GetAttributePointsReward_Implementation(int32 InLevel) const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->LevelUpInfo->LevelUpInformation[InLevel].AttributePointAward;
}

int32 ACavePlayerCharacter::GetSpellPointsReward_Implementation(int32 InLevel) const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->LevelUpInfo->LevelUpInformation[InLevel].SpellPointAward;
}

void ACavePlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddTpXP(InXP);
}

void ACavePlayerCharacter::AddToPlayerLevel_Implementation(int32 InLevel)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddToLevel(InLevel);

	if (UCaveAbilitySystemComponent* CaveASC = Cast<UCaveAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		CaveASC->UpdateAbilityStatuses(CavePlayerState->GetPlayerLevel());
	}
}

void ACavePlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddToSpellPoints(InSpellPoints);
}

void ACavePlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddToAttributePoints(InAttributePoints);
}

void ACavePlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void ACavePlayerCharacter::InPlayerComboAttack_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 90.f, 0.f);
}

void ACavePlayerCharacter::EndPlayerComboAttack_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseMaxWalkSpeed;
	GetCharacterMovement()->RotationRate = BaseRotationRate;
}

void ACavePlayerCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (ACavePlayerController* CavePlayerController =  Cast<ACavePlayerController>(GetController()))
	{
		CavePlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void ACavePlayerCharacter::HideMagicCircle_Implementation()
{
	if (ACavePlayerController* CavePlayerController =  Cast<ACavePlayerController>(GetController()))
	{
		CavePlayerController->HideMagicCircle();
	}
}

void ACavePlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = FollowCamera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

void ACavePlayerCharacter::InitAbilityActorInfo()
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	if (!CavePlayerState) return;
	
	CavePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CavePlayerState, this);

	AbilitySystemComponent = CavePlayerState->GetAbilitySystemComponent();
	AttributeSet = CavePlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);

	if (ACavePlayerController* CavePlayerController =  Cast<ACavePlayerController>(GetController()))
	{
		if (ACaveHUD* CaveHUD = Cast<ACaveHUD>(CavePlayerController->GetHUD()))
		{
			CaveHUD->InitOverlay(CavePlayerController, CavePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	
	FCaveGameplayTags GameplayTags = FCaveGameplayTags::Get();

	
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Abilities_Common_HitReact)
		.AddUObject(this, &ACavePlayerCharacter::HitReactTagChange);

	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Abilities_Common_Death)
		.AddUObject(this, &ACavePlayerCharacter::DeathReactTagChange);
	
	InitializeDefaultAttributes();
	
}

void ACavePlayerCharacter::HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
}

void ACavePlayerCharacter::DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::DeathReactTagChange(CallbackTag, NewCount);
	
}


