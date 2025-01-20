// Copyright Jonwoo-Choi


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "AbilitySystem/CaveAttributeSet.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
	OnHealthChanged.Broadcast(GetCaveAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetCaveAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetCaveAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetCaveAttributeSet()->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
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

