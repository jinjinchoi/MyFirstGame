// Copyright Jonwoo-Choi


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(const int32 XP) const
{
	int32 Level = 1;

	// LevelUpInformation[Level] = 해당 Level에 필요한 XP 정보가 있음
	while (Level < LevelUpInformation.Num() && XP >= LevelUpInformation[Level].LevelUpRequirement)
	{
		++Level;
	}

	return Level;
}
