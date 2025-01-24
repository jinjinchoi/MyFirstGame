// Copyright Jonwoo-Choi


#include "Actor/CaveFireBlade.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CaveFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ACaveFireBlade::ACaveFireBlade()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 350.f;
	ProjectileMovement->MaxSpeed = 350.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ACaveFireBlade::Destroyed()
{
	GetWorld()->GetTimerManager().ClearTimer(OverlapTimerHandle);
	
	Super::Destroyed();
	
}


void ACaveFireBlade::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);

	GetWorld()->GetTimerManager().SetTimer(OverlapTimerHandle, this, &ACaveFireBlade::ApplyGameplayEffect, DamageInterval, true, 0.f);
	
}


void ACaveFireBlade::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	OverlappingActors.AddUnique(OtherActor);
}

void ACaveFireBlade::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingActors.Remove(OtherActor);
}

void ACaveFireBlade::ApplyGameplayEffect()
{
	if (!HasAuthority()) return;
	
	for (AActor* OtherActor : OverlappingActors)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if (DamageEffectParams.bIsKnockback)
			{
				FRotator Rotation = GetActorRotation();
				const FVector KnockbackDirection = Rotation.Vector();
				DamageEffectParams.KnockbackDirection = KnockbackDirection * DamageEffectParams.KnockbackForceMafnitude;
			}
			DamageEffectParams.TartgetAbilitySystemComponent = TargetASC;
			UCaveFunctionLibrary::ApplyDamageEffect(DamageEffectParams);
			OnHit();
		}
	}
}



void ACaveFireBlade::OnHit_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
}


bool ACaveFireBlade::IsValidOverlap(AActor* OtherActor) const
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;

	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	if (!UCaveFunctionLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return false;

	return true;
}
