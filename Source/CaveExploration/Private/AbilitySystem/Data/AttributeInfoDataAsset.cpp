// Copyright Jonwoo-Choi


#include "AbilitySystem/Data/AttributeInfoDataAsset.h"

FCaveAttributeInfo UAttributeInfoDataAsset::FindAttributeInfoForTag(const FGameplayTag& InAttributeTag) const
{
	for (const FCaveAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTag(InAttributeTag))
		{
			return Info;
		}
	}
	return FCaveAttributeInfo();
}
