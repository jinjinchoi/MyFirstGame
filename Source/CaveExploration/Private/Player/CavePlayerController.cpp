// Copyright Jonwoo-Choi


#include "Player/CavePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CaveFunctionLibrary.h"
#include "CaveGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Input/CaveInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
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

	if (IsValid(GetPawn()))
	{
		ActiveSpringArm = Cast<USpringArmComponent>(GetPawn()->GetComponentByClass(USpringArmComponent::StaticClass()));
		ActiveCameraComponent = Cast<UCameraComponent>(GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));
		ActiveCapsuleComponent = Cast<UCapsuleComponent>(GetPawn()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	}

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

void ACavePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACavePlayerController, MagicCircle);
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
		if (ControlledPawn->Implements<UPlayerInterface>())
		{
			const FVector NewMoveDirection = ForwardDirection * InputAxisVector.Y + RightDirection * InputAxisVector.X;
			IPlayerInterface::Execute_SetCharacterMoveDirection(ControlledPawn, NewMoveDirection);
		}
		
		if (!GetASC() || GetASC()->HasMatchingGameplayTag(FCaveGameplayTags::Get().Player_Block_Move)) return;

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

void ACavePlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (IsValid(MagicCircle)) return;

	if (HasAuthority())
	{
		const FVector SpawnedMagicCircleLocation = GetPawn()->GetActorLocation();
		FActorSpawnParameters SpawnParameters;
		if (APawn* OwningPawn = GetPawn())
		{
			SpawnParameters.Owner = OwningPawn;
		}
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass, SpawnedMagicCircleLocation, FRotator::ZeroRotator, SpawnParameters);
		
	}
	
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

FVector ACavePlayerController::GetValidMagicCircleLocation() const
{
	if (IsValid(MagicCircle))
	{
	
		const FVector MagicCircleLocation = MagicCircle->GetActorLocation();
		
		const FVector RaisedLocation = FVector(MagicCircleLocation.X, MagicCircleLocation.Y, MagicCircleLocation.Z + 500);
		const FVector LoweredLocation = FVector(MagicCircleLocation.X, MagicCircleLocation.Y, MagicCircleLocation.Z - 500);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UCaveFunctionLibrary::GetLivePlayerWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f,  MagicCircleLocation);
		IgnoreActors.Add(MagicCircle);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		FVector AdjustedLocation = MagicCircleLocation;  // 기본값 설정
		if (HitResult.bBlockingHit)
		{
			AdjustedLocation.Z = HitResult.ImpactPoint.Z;

			return AdjustedLocation;
		}
	}
	
	return FVector::ZeroVector;
}



/////////////////////////////////////////////////////////
///////* To make actor translucent */////////////////////
/////////////////////////////////////////////////////////

void ACavePlayerController::SyncOccludedActors()
{
	if (!ShouldCheckCameraOcclusion()) return;

	/**
	 *  bDoCollisionTest가 ture면 카메라와 캐릭터 사이에 장애물이 존재할 수 없음.
	 *  그렇기 때문에 조기 종료
	 */  
	if (ActiveSpringArm->bDoCollisionTest)
	{
		ForceShowOccludedActor();
		return;
	}

	const FVector Start = ActiveCameraComponent->GetComponentLocation();
	const FVector End = GetPawn()->GetActorLocation();

	/**
	 * 충돌을 감지할 객체의 타입을 배열에 저장
	 */
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;

	const EDrawDebugTrace::Type ShouldDebug = DebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	const bool bGoHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		GetWorld(),
		Start,
		End,
		ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
		ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace,
		CollisionObjectTypes,
		false,
		ActorsToIgnore,
		ShouldDebug,
		OutHits,
		true
	);

	if (bGoHits)
	{
		TSet<const AActor*> ActorsJustOccluded;

		for (FHitResult HitResult : OutHits)
		{
			const AActor* HitActor = HitResult.GetActor();
			HideOccludedActor(HitActor);
			ActorsJustOccluded.Add(HitActor);
		}

		/**
		 * 숨겨진 액터를 순회 하면서 방금 저장된 액터가 아니면 다시 표시
		 */
		for (TTuple<const AActor*, FCameraOccludedActor>& Elem : OccludedActors)
		{
			if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.IsOccluded)
			{
				ShowOccludedActor(Elem.Value);
			}

			if (DebugLineTraces)
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, but it's not occluded anymore with the new hits."), *Elem.Value.Actor->GetName());
			}
		}
	}
	else
	{
		ForceShowOccludedActor();
	}
	
}

void ACavePlayerController::HideOccludedActor(const AActor* Actor)
{
	FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);

	if (ExistingOccludedActor && ExistingOccludedActor->IsOccluded)
	{
		if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was already occluded. Ignoring."), *Actor->GetName());
		return;
	}

	if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
	{
		ExistingOccludedActor->IsOccluded = true;
		OnHideOccludedActor(*ExistingOccludedActor);

		if (DebugLineTraces)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor %s exists, but was not occluded. Occluding it now."), *Actor->GetName());
		}
	}
	else
	{
		UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		FCameraOccludedActor OccludedActor;
		OccludedActor.Actor = Actor;
		OccludedActor.StaticMesh = StaticMesh;
		OccludedActor.Materials = StaticMesh->GetMaterials();
		OccludedActor.IsOccluded = true;
		OccludedActors.Add(Actor, OccludedActor);
		OnHideOccludedActor(OccludedActor);

		if (DebugLineTraces)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor %s does not exist, creating and occluding it now."), *Actor->GetName());
		}
	}
}

void ACavePlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
	{
		OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
	}
}

void ACavePlayerController::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
	if (!IsValid(OccludedActor.Actor))
	{
		OccludedActors.Remove(OccludedActor.Actor);
	}

	OccludedActor.IsOccluded = false;
	OnShowOccludedActor(OccludedActor);
}

void ACavePlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int MatIndex = 0; MatIndex < OccludedActor.Materials.Num(); ++MatIndex)
	{
		OccludedActor.StaticMesh->SetMaterial(MatIndex, OccludedActor.Materials[MatIndex]);
	}
}

void ACavePlayerController::ForceShowOccludedActor()
{
	for (auto& Elem : OccludedActors)
	{
		if (Elem.Value.IsOccluded)
		{
			ShowOccludedActor(Elem.Value);
			if (DebugLineTraces)
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, force to show again."), *Elem.Value.Actor->GetName());
			}
		}
	}
}
