// Copyright Jonwoo-Choi

#define CREATE_AND_SETUP_PT(Pt_X)\
	Pt_X = CreateDefaultSubobject<USceneComponent>(TEXT(#Pt_X));\
	ImmutablePts.Add(Pt_X); \
	Pt_X->SetupAttachment(GetRootComponent());

#include "Actor/PointCollection.h"

#include "CaveFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{

	PrimaryActorTick.bCanEverTick = false;

	Pt_Root = CreateDefaultSubobject<USceneComponent>("Pt_Root");
	ImmutablePts.Add(Pt_Root);
	SetRootComponent(Pt_Root);

	CREATE_AND_SETUP_PT(Pt_1);
	CREATE_AND_SETUP_PT(Pt_2);
	CREATE_AND_SETUP_PT(Pt_3);
	CREATE_AND_SETUP_PT(Pt_4);
	CREATE_AND_SETUP_PT(Pt_5);
	CREATE_AND_SETUP_PT(Pt_6);
	CREATE_AND_SETUP_PT(Pt_7);
	CREATE_AND_SETUP_PT(Pt_8);
	CREATE_AND_SETUP_PT(Pt_9);
	CREATE_AND_SETUP_PT(Pt_10);


}

TArray<USceneComponent*> APointCollection::GetGroundPoints(int32 NumPoints, const float YawOverride)
{
	check(ImmutablePts.Num() > 0);

	if (ImmutablePts.Num() < NumPoints)
	{
		NumPoints = ImmutablePts.Num();
	}

	TArray<USceneComponent*> ArrayCopy;
	TArray<AActor*> IgnoreActors;
	UCaveFunctionLibrary::GetLivePlayerWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

	for (USceneComponent* Pt : ImmutablePts)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Pt != Pt_Root)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_Root->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Pt_Root->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Pt);
	}

	return ArrayCopy;
	
}


void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

