// Copyright Jonwoo-Choi


#include "Character//CavePlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "CaveGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "Player/CavePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CavePlayerController.h"
#include "UI/HUD/CaveHUD.h"

ACavePlayerCharacter::ACavePlayerCharacter()
{
	Tags.Add(FName("Player"));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 750.f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
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

void ACavePlayerCharacter::InitAbilityActorInfo()
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CavePlayerState, this);

	AbilitySystemComponent = CavePlayerState->GetAbilitySystemComponent();
	AttributeSet = CavePlayerState->GetAttributeSet();

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
