// Copyright Jonwoo-Choi


#include "AbilitySystem/Abilities/CaveGameplayAbility.h"

#include "AbilitySystem/CaveAttributeSet.h"

FGameplayTagContainer* UCaveGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset();
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UCaveGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Cooldown.Cave")), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

float UCaveGameplayAbility::GetManaCost(const float AbilityLevel) const
{
	float ManaCost = 0.0f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (const FGameplayModifierInfo& Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UCaveAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(AbilityLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UCaveGameplayAbility::GetCoolDown(const float AbilityLevel) const
{
	return CooldownDuration.GetValueAtLevel(AbilityLevel);
}

float UCaveGameplayAbility::GetDamage(float AbilityLevel) const
{
	return 0.f;
}
