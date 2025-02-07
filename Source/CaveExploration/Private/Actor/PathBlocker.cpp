// Copyright Jonwoo-Choi


#include "Actor/PathBlocker.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"


APathBlocker::APathBlocker()
{

	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetGenerateOverlapEvents(false);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara Component");
	NiagaraComponent->SetupAttachment(GetRootComponent());
	NiagaraComponent->bAutoActivate = false;
	NiagaraComponent->SetAutoDestroy(false);
	NiagaraComponent->SetGenerateOverlapEvents(false);
	NiagaraComponent->SetIsReplicated(true);

}

void APathBlocker::ActivateBlocker() const
{
	if (BoxComponent)
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
		BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	}

	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);
	}
	
}

void APathBlocker::DeactivateBlocker() const
{
	if (BoxComponent)
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}
}


void APathBlocker::BeginPlay()
{
	Super::BeginPlay();
	
	
}


