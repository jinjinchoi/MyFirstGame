// Copyright Jonwoo-Choi


#include "UI/WidgetController/CaveWidgetController.h"

#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "AbilitySystem/CaveAttributeSet.h"
#include "Player/CavePlayerState.h"
#include "Player/CavePlayerController.h"

void UCaveWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UCaveWidgetController::BroadCastInitialValues()
{
}

void UCaveWidgetController::BindCallbacksToDependencies()
{
}

void UCaveWidgetController::BroadCastAbilityInfo()
{
	if (!GetCaveAbilitySystemComponent()->bStartupAbilitiesGiven) return;

	FForEachAbilityDelegate BroadCastDelegate;
	BroadCastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FCaveAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(UCaveAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = UCaveAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetCaveAbilitySystemComponent()->ForEachAbility(BroadCastDelegate);
}

ACavePlayerController* UCaveWidgetController::GetCavePlayerController()
{
	if (CavePlayerController == nullptr)
	{
		CavePlayerController = Cast<ACavePlayerController>(PlayerController);
	}

	return CavePlayerController;
}

ACavePlayerState* UCaveWidgetController::GetCavePlayerState()
{
	if (CavePlayerState == nullptr)
	{
		CavePlayerState = Cast<ACavePlayerState>(PlayerState);
	}

	return CavePlayerState;
}

UCaveAbilitySystemComponent* UCaveWidgetController::GetCaveAbilitySystemComponent()
{
	if (CaveAbilitySystemComponent == nullptr)
	{
		CaveAbilitySystemComponent = Cast<UCaveAbilitySystemComponent>(AbilitySystemComponent);
	}

	return CaveAbilitySystemComponent;
}

UCaveAttributeSet* UCaveWidgetController::GetCaveAttributeSet()
{
	if (CaveAttributeSet == nullptr)
	{
		CaveAttributeSet = Cast<UCaveAttributeSet>(AttributeSet);
	}

	return CaveAttributeSet;
}
