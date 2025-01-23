// Copyright Jonwoo-Choi


#include "AbilitySystem/Abilities/CaveDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


float UCaveDamageGameplayAbility::GetDamage(const float AbilityLevel) const
{
	return Damage.GetValueAtLevel(AbilityLevel);
}

FDamageEffectParams UCaveDamageGameplayAbility::MakeDamageEffectParams(AActor* TargetActor,
                                                                       bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams Params;

	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TartgetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.AbilityLevel = GetAbilityLevel();
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.DamageType = DamageType;
	
	if (DebuffChance > 0.f)
	{
		Params.DebuffChance = DebuffChance;
		Params.DebuffDamage = DebuffDamage;
		Params.DebuffDuration = DebuffDuration;
		Params.DebuffFrequncy = DebuffFrequncy;
	}

	const bool bIsKnockback = FMath::FRandRange(0.f, 100.f) < KnockbackChance;
	if (bIsKnockback)
	{
		Params.bIsKnockback = bIsKnockback;
		Params.KnockbackForceMafnitude = KnockbackForceMafnitude;
	}

	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		if (!bOverrideKnockbackDirection)
		{
			Params.KnockbackDirection = ToTarget * KnockbackForceMafnitude;
		}
	}

	if (bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		Params.KnockbackDirection = KnockbackDirectionOverride * KnockbackForceMafnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			Params.KnockbackDirection = KnockbackRotation.Vector() * KnockbackForceMafnitude;
		}
	}

	
	return Params;
}
