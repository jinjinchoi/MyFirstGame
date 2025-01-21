// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CaveAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenDelegate);
DECLARE_DELEGATE_OneParam(FForEachAbilityDelegate, const FGameplayAbilitySpec&);

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FAbilitiesGivenDelegate AbilitiesGivenDelegate;
	bool bStartupAbilitiesGiven = false;
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbilityDelegate& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	
protected:
	/* Ability System Component */
	virtual void OnRep_ActivateAbilities() override;
	/* end Ability System Component */
};

