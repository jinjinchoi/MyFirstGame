// Copyright Jonwoo-Choi


#include "Actor/CaveProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CaveFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h" 
#include "CaveExploration/CaveExploration.h"


ACaveProjectile::ACaveProjectile()
{

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("CollisionBox");
	SetRootComponent(SphereCollision);
	SphereCollision->SetCollisionObjectType(ECC_Projectile);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	if (ProjectileMovement->InitialSpeed == 0.f || ProjectileMovement->MaxSpeed == 0.f)
	{
		ProjectileMovement->InitialSpeed = 400.f;
		ProjectileMovement->MaxSpeed = 400.f;
		ProjectileMovement->ProjectileGravityScale = 0.f;
	}

}


void ACaveProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACaveProjectile::OnSphereOverlap);
	
}


void ACaveProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) OnHit();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if (DamageEffectParams.bIsKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = KnockbackPitch;
				const FVector KnockbackDirection = Rotation.Vector();
				DamageEffectParams.KnockbackDirection = KnockbackDirection * DamageEffectParams.KnockbackForceMafnitude;
			}
			DamageEffectParams.TartgetAbilitySystemComponent = TargetASC;
			UCaveFunctionLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

void ACaveProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	bHit = true;
}

void ACaveProjectile::Destroyed()
{
	if (!bHit && !HasAuthority()) OnHit();

	Super::Destroyed();
}

bool ACaveProjectile::IsValidOverlap(AActor* OtherActor) const
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	if (SourceAvatarActor == OtherActor) return false;
	if (!UCaveFunctionLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return false;

	return true;
}






