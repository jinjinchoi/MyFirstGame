// Copyright Jonwoo-Choi


#include "UI/WidgetController/AttributeWidgetController.h"

#include "AbilitySystem/CaveAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfoDataAsset.h"
#include "Player/CavePlayerState.h"

void UAttributeWidgetController::BindCallbacksToDependencies()
{
	UCaveAttributeSet* AS = CastChecked<UCaveAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (const FCaveAttributeInfo& Info : AttributeInfo->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.AttributeGetter).AddLambda([this, Info](const FOnAttributeChangeData& Data)
		{
			BroadCastAttributeInfo(Info.AttributeTag);
		});
	}

	GetCavePlayerState()->OnAttributePointsChangedDelegate.AddLambda([this](int32 NewValue)
	{
		AttributeChangedDelegate.Broadcast(NewValue);
	});
}

void UAttributeWidgetController::BroadCastInitialValues()
{
	check(AttributeInfo);

	for (const FCaveAttributeInfo& Info : AttributeInfo->AttributeInformation)
	{
		BroadCastAttributeInfo(Info.AttributeTag);
	}
}

void UAttributeWidgetController::BroadCastAttributeInfo(const FGameplayTag& AttributeTag) const
{
	FCaveAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
	
}
