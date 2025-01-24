// Copyright Jonwoo-Choi


#include "Actor/MagicCircle.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = false;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	SetRootComponent(MagicCircleDecal);

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

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

void AMagicCircle::Move(const FVector2D& Value)
{
	if (Value.IsNearlyZero()) return;

	const FRotator Rotation = FRotator::ZeroRotator;
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); 

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 입력 값에 따른 이동 벡터 계산
	const FVector Movement = (ForwardDirection * Value.Y + RightDirection * Value.X).GetClampedToMaxSize(1.0f);  // 정규화 (속도 보정)

	// 이동 적용
	AddActorWorldOffset(Movement * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
}


void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

