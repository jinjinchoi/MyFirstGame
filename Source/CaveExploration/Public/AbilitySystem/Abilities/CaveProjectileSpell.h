// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CaveDamageGameplayAbility.h"
#include "CaveProjectileSpell.generated.h"

class ACaveProjectile;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveProjectileSpell : public UCaveDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category=Projectile)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const bool bOverridePitch = false, const float PitchOverride = 0, AActor* HomingTarget = nullptr);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Projectile)
	TSubclassOf<ACaveProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 400.f;
	
private:

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	float HomingAccelerationMin = 2600.f;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	float HomingAccelerationMax = 4200.f;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	int32 MinNumProjectiles = 1;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	int32 MaxNumProjectiles = 5;
	
	
};
