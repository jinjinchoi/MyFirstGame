// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "CaveAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
