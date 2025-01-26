// Copyright Jonwoo-Choi


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "CaveFunctionLibrary.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "Player/CavePlayerState.h"

void USpellMenuWidgetController::BroadCastInitialValues()
{
	BroadCastAbilityInfo();
	SpellPointsChangedDelegate.Broadcast(GetCavePlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	
	GetCaveAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
	{

		FString Description;
		FString NextLevelDescription;
		
		if (AbilityInfo)
		{
			FCaveAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);

			// 스킬 설명 불러오기
			Description = UCaveFunctionLibrary::GetAbilityDescription(this, AbilityTag, AbilityLevel, Info);
			NextLevelDescription = UCaveFunctionLibrary::GetAbilityDescription(this, AbilityTag, AbilityLevel + 1, Info);
		}

		if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.StatusTag = StatusTag;
			bool bEnableSpendPoints = false;
			ShouldEnableButton(AbilityTag, StatusTag, CurrentSpellPoints, bEnableSpendPoints);
			
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, Description, NextLevelDescription);
		}
	});

	GetCavePlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChangedDelegate.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;
		bool bEnableSpendPoints = false;
		ShouldEnableButton(SelectedAbility.AbilityTag, SelectedAbility.StatusTag, CurrentSpellPoints, bEnableSpendPoints);

		// 스킬 설명 불러오기
		FString Description;
		FString NextLevelDescription;
		if (AbilityInfo)
		{
			const FCaveAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag);
			const int32 AbilityLevel = GetCaveAbilitySystemComponent()->GetAbilityLevelFromAbilityTag(SelectedAbility.AbilityTag);
			Description = UCaveFunctionLibrary::GetAbilityDescription(this, SelectedAbility.AbilityTag, AbilityLevel, Info);
			NextLevelDescription = UCaveFunctionLibrary::GetAbilityDescription(this, SelectedAbility.AbilityTag, AbilityLevel + 1, Info);
		}
		
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, Description, NextLevelDescription);
	});

	GetCaveAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();
	
	const int32 SpellPoints = GetCavePlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetCaveAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		// 태그가 유효하지 않거나 어빌리티 태그에 해당하는 능력이 활성화 되어 있지 않다는 것은 잠겨있다는 것이다.
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		// 능력이 잠겨있지 않으면 Status 태그를 가져온다.
		AbilityStatus = GetCaveAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec);
	}

	// 선택한 Spell Globe의 태그들을 저장
	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;

	bool bEnableSpendPoints = false;
	ShouldEnableButton(AbilityTag, AbilityStatus, SpellPoints, bEnableSpendPoints);

	// 스킬 설명 불러오기
	FString Description;
	FString NextLevelDescription;
	if (AbilityInfo)
	{
		const FCaveAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag);
		const int32 AbilityLevel = GetCaveAbilitySystemComponent()->GetAbilityLevelFromAbilityTag(SelectedAbility.AbilityTag);
		Description = UCaveFunctionLibrary::GetAbilityDescription(this, SelectedAbility.AbilityTag, AbilityLevel, Info);
		NextLevelDescription = UCaveFunctionLibrary::GetAbilityDescription(this, SelectedAbility.AbilityTag, AbilityLevel + 1, Info);
	}
	
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, Description, NextLevelDescription);

	PrepareEquipping();
	
}


void USpellMenuWidgetController::PrepareEquipping()
{
	if (SelectedAbility.AbilityTag == FCaveGameplayTags::Get().Abilities_Spell_Physical_MeleeAttack)
	{
		bWaitingForEquipSelection = false;
		return;
	}
	
	bWaitingForEquipSelection = true;

	const FGameplayTag& SelectedStatus = GetCaveAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedAbility.AbilityTag);
	if (SelectedStatus.MatchesTagExact(FCaveGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetCaveAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& InputTag)
{
	if (!bWaitingForEquipSelection) return;

	GetCaveAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.AbilityTag, InputTag);
	
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{

	const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();
	
	bWaitingForEquipSelection = false;
	FCaveAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	LastSlotInfo.InputTag = PreviousSlot;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FCaveAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
	
	GlobeDeselect();
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
}


void USpellMenuWidgetController::ShouldEnableButton(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bShouldEnableSpellPointsButton)
{
	const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();

	if (!AbilityStatus.MatchesTag(GameplayTags.Abilities_Status))
	{
		bShouldEnableSpellPointsButton = false;
		return;
	}

	const int32 AbilityLevel = GetCaveAbilitySystemComponent()->GetAbilityLevelFromAbilityTag(AbilityTag);
	if (AbilityLevel >= 10)
	{
		bShouldEnableSpellPointsButton = false;
		return;
	}

	if (SpellPoints <= 0 || AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bShouldEnableSpellPointsButton = false;
	}
	else
	{
		bShouldEnableSpellPointsButton = true;
	}
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	GetCaveAbilitySystemComponent()->ServerSpellSpendPoint(SelectedAbility.AbilityTag);
}

void USpellMenuWidgetController::GlobeDeselect()
{
	
	SelectedAbility.AbilityTag = FCaveGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag = FCaveGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, FString(), FString());

	if (bWaitingForEquipSelection)
	{
		bWaitingForEquipSelection = false;
	}
}
