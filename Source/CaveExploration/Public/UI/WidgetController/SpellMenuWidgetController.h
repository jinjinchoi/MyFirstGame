// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "CaveGameplayTags.h"
#include "UI/WidgetController/CaveWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSpellGlobeSelectedDelegate, bool, bSpendPointButtonEnabled, const FString&, Description, const FString&, NextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedDelegate, const FGameplayTag&, AbilityTag);
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API USpellMenuWidgetController : public UCaveWidgetController
{
	GENERATED_BODY()

public:
	/* Cave Widget Controller */
	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangeDelegate SpellPointsChangedDelegate;
	/* end Cave Widget Controller */

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedDelegate SpellGlobeSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedDelegate SpellGlobeReassignedDelegate;

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

	void PrepareEquipping();

	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& InputTag);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);
	

private:
	void ShouldEnableButton(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton);
	FSelectedAbility SelectedAbility = { FCaveGameplayTags::Get().Abilities_None, FCaveGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	bool bWaitingForEquipSelection = false;
	FGameplayTag SelectedSlot;
	
};


