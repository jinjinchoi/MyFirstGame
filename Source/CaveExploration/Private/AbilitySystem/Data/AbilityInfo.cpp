// Copyright Jonwoo-Choi


#include "AbilitySystem/Data/AbilityInfo.h"

FCaveAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag)
{
	for (const FCaveAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}

	return FCaveAbilityInfo();
}
