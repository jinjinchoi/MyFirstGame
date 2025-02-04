// Copyright Jonwoo-Choi


#include "CheckPoint/CheckPoint.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(GetRootComponent());
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToChannels(ECR_Block);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetupAttachment(CheckPointMesh);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(CheckPointMesh);
	NiagaraComponent->bAutoActivate = false;

	SoundComponent = CreateDefaultSubobject<UAudioComponent>("SoundComponent");
	SoundComponent->bAutoActivate = false;
	SoundComponent->SetupAttachment(GetRootComponent());
	
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACheckPoint::OnSphereEndOverlap);
	if (SoundComponent && LoopingSound)
	{
		SoundComponent->SetSound(LoopingSound);
	}
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;
	
	ApplyEffectToTarget(OtherActor);
	
	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);
	}
	
	if (SoundComponent)
	{
		SoundComponent->Play();
	}

	
	if (!OtherActor->HasAuthority()) return;
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag, CheckPointName);
	}
}

void ACheckPoint::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (NiagaraComponent && OtherActor->ActorHasTag("Player"))
	{
		NiagaraComponent->Deactivate();
	}
	
	if (SoundComponent)
	{
		SoundComponent->Stop();
	}
}

void ACheckPoint::ApplyEffectToTarget(AActor* TargetActor)
{
	if (!GameplayEffectClass || !TargetActor->ActorHasTag("Player")) return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, ContextHandle);
		
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
}
