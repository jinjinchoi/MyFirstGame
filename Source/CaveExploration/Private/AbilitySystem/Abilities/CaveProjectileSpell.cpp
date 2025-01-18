// Copyright Jonwoo-Choi


#include "AbilitySystem/Abilities/CaveProjectileSpell.h"

#include "CaveFunctionLibrary.h"
#include "Actor/CaveProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

void UCaveProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UCaveProjectileSpell::SpawnProjectiles(const FVector& ProjectileTargetLocation,
	bool bOverridePitch, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority() || !GetAvatarActorFromActorInfo()->Implements<UCombatInterface>()) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo());
	
	FRotator Rotation = FRotator::ZeroRotator;
	if (ProjectileTargetLocation.IsZero())
	{
		// 타겟이 없을 경우 (플레이어) 정면으로 발사체 발사 
		Rotation = UKismetMathLibrary::MakeRotFromX(GetAvatarActorFromActorInfo()->GetActorForwardVector());
	}
	else
	{
		// 타겟이 있으면 (에너미 또는 유도 공격) 해당 방향으로 발사체 발사
		Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	}
	
	if (bOverridePitch)
	{
		// TODO: 거리와 속도 계산해서 각도 계산해야함
		// Rotation.Pitch = ??
	}

	const int32 NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	const FVector Forward = Rotation.Vector();

	TArray<FRotator> Rotations =  UCaveFunctionLibrary::EvenlySpaceRotators(Forward, ProjectileSpread, NumProjectiles);
	

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		ACaveProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACaveProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
	
		if (IsValid(HomingTarget) && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}

		Projectile->DamageEffectParams = MakeDamageEffectParams();
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = HomingTarget ? true : false;

		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
