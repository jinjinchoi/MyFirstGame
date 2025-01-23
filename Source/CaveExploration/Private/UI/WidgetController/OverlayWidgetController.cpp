// Copyright Jonwoo-Choi


#include "UI/WidgetController/OverlayWidgetController.h"

#include "CaveGameplayTags.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "AbilitySystem/CaveAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/CavePlayerState.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
	OnHealthChanged.Broadcast(GetCaveAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetCaveAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetCaveAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetCaveAttributeSet()->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetCavePlayerState()->OnXPChangeDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetCavePlayerState()->OnLevelChangeDelegate.AddLambda([this](const int32 NewLevel, const bool bIsLevelUp)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bIsLevelUp);
	});
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCaveAttributeSet()->GetHealthAttribute()).
	AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCaveAttributeSet()->GetMaxHealthAttribute()).
	AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCaveAttributeSet()->GetManaAttribute()).
	AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCaveAttributeSet()->GetMaxManaAttribute()).
	AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});

	if (GetCaveAbilitySystemComponent())
	{
		GetCaveAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		
		if (GetCaveAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadCastAbilityInfo();
		}
		else
		{
			GetCaveAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadCastAbilityInfo);
		}
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetCavePlayerState()->LevelUpInfo;

	check(LevelUpInfo);

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num() - 1;

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();
	
	FCaveAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	LastSlotInfo.InputTag = PreviousSlot;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FCaveAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}

