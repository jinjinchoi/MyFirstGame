// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CaveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	FGameplayTag StartupInputTag;
	
	virtual FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Cooldown, meta=(Categories = "Cooldown"))
	FGameplayTagContainer CooldownTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Cooldown)
	FScalableFloat CooldownDuration;

	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
	
};
