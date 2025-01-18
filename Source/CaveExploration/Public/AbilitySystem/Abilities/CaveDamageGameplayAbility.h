// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "CaveAbilityTypes.h"
#include "AbilitySystem/Abilities/CaveGameplayAbility.h"
#include "CaveDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveDamageGameplayAbility : public UCaveGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParams(
		AActor* TargetActor = nullptr,
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f
	) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffFrequncy = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FVector KnockbackDirection = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackForceMafnitude = 1000.f;
};
