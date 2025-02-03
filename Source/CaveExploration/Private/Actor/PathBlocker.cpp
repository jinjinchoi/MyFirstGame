// Copyright Jonwoo-Choi


#include "Actor/PathBlocker.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"


APathBlocker::APathBlocker()
{

	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara Component");
	NiagaraComponent->SetupAttachment(GetRootComponent());
	NiagaraComponent->bAutoActivate = false;
	

}

void APathBlocker::ActivateEffect() const
{
	NiagaraComponent->Activate(true);
}

void APathBlocker::DeactivateEffect() const
{
	NiagaraComponent->Deactivate();
}


void APathBlocker::BeginPlay()
{
	Super::BeginPlay();
	
}

