// Copyright Jonwoo-Choi


#include "CheckPoint/CheckPoint.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(GetRootComponent());
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetupAttachment(CheckPointMesh);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(CheckPointMesh);
	NiagaraComponent->bAutoActivate = false;
	
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACheckPoint::OnSphereEndOverlap);
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);
	}
}

void ACheckPoint::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(false);
	}
}
