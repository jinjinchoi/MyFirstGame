// Copyright Jonwoo-Choi


#include "Player/CavePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CaveGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"
#include "Input/CaveInputComponent.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextWidgetComponent.h"

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

	OriginalCharacter = GetPawn();

}

void ACavePlayerController::ShowDamageNumber_Implementation(const float DamageAmount, ACharacter* TargetCharacter, const bool bIsCriticalHit, const FGameplayTag& DamageType)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextWidgetComponent* DamageTextComponent = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageAmount, bIsCriticalHit, DamageType);
		
	}
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
	const FVector2D InputAxisVector = InputValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (IsValid(MagicCircle))
	{
		MagicCircle->Move(InputAxisVector);
	}
	else if (APawn* ControlledPawn  = GetPawn<APawn>())
	{
		if (!GetASC() || GetASC()->HasMatchingGameplayTag(FCaveGameplayTags::Get().Player_Block_InputPressed)) return;

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

void ACavePlayerController::ServerPossessCharacter_Implementation(APawn* NewPawn)
{
	if (IsValid(NewPawn))
	{
		Possess(NewPawn);
	}
}

void ACavePlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
	
	const FVector MagicCircleLocation = GetPawn()->GetActorLocation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	
	MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass, MagicCircleLocation, FRotator::ZeroRotator, SpawnParameters);
	if (DecalMaterial)
	{
		MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
	}
}

void ACavePlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}