// Copyright Jonwoo-Choi


#include "AbilitySystem/CaveAbilitySystemGlobals.h"

#include "CaveAbilityTypes.h"

FGameplayEffectContext* UCaveAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCaveGameplayEffectContext();
}
