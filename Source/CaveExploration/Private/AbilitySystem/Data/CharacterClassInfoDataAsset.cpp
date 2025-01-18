// Copyright Jonwoo-Choi


#include "AbilitySystem/Data/CharacterClassInfoDataAsset.h"

FCharacterClassDefaultInfo UCharacterClassInfoDataAsset::GetClassDefaultInfo(ECharacterClass CharacterClass) const
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
