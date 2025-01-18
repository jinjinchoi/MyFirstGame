// Copyright Jonwoo-Choi


#include "Player/CavePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CaveGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "Input/CaveInputComponent.h"

ACavePlayerController::ACavePlayerController()
{
	bReplicates = true;
}

void ACavePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(CaveContext);

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(CaveContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ACavePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UCaveInputComponent* CaveInputComponent = CastChecked<UCaveInputComponent>(InputComponent);
	CaveInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACavePlayerController::Move);
	CaveInputComponent->BindAbilityActions(
		InputConfig,
		this,
		&ACavePlayerController::AbilityInputTagPressed,
		&ACavePlayerController::AbilityInputTagReleased,
		&ACavePlayerController::AbilityInputTagHeld
	);
}

void ACavePlayerController::Move(const FInputActionValue& InputValue)
{
	if (!GetASC() || GetASC()->HasMatchingGameplayTag(FCaveGameplayTags::Get().Player_Block_InputPressed)) return;
	
	const FVector2D InputAxisVector = InputValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn  = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ACavePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!GetASC() || GetASC()->HasMatchingGameplayTag(FCaveGameplayTags::Get().Player_Block_InputPressed)) return;
	
	GetASC()->AbilityInputPressed(InputTag);
}

void ACavePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetASC() || GetASC()->HasMatchingGameplayTag(FCaveGameplayTags::Get().Player_Block_Released)) return;
	
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ACavePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetASC() || GetASC()->HasMatchingGameplayTag(FCaveGameplayTags::Get().Player_Block_InputHeld)) return;
	
	GetASC()->AbilityInputTagHeld(InputTag);
	
}

UCaveAbilitySystemComponent* ACavePlayerController::GetASC()
{
	if (!CaveAbilitySystemComponent)
	{
		CaveAbilitySystemComponent = Cast<UCaveAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return CaveAbilitySystemComponent;
}
