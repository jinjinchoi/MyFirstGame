// Copyright Jonwoo-Choi


#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = false;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	SetRootComponent(MagicCircleDecal);
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	SetReplicates(true);
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

	const FVector NewLocation = GetActorLocation() + (Movement * MoveSpeed * GetWorld()->GetDeltaSeconds());

	// 부드러운 움직임을 위하여 로컬에서 먼저 이동
	if (!HasAuthority()) 
	{
		SetActorLocation(NewLocation);
	}
	
	ServerMove(NewLocation);
	
}


void AMagicCircle::ServerMove_Implementation(const FVector& InMovement)
{
	SetActorLocation(InMovement);
}

