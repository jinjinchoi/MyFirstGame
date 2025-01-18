// Copyright Jonwoo-Choi


#include "Input/CaveInputConfig.h"

const UInputAction* UCaveInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FCaveInputAction& Action : InputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return  Action.InputAction;
		}
	}
	return nullptr;
}
