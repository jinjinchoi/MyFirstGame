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
#include "Components/BoxComponent.h"


ACaveProjectile::ACaveProjectile()
{

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	SetRootComponent(BoxCollision);
	BoxCollision->SetCollisionObjectType(ECC_Projectile);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}


void ACaveProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACaveProjectile::OnSphereOverlap);
	
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




