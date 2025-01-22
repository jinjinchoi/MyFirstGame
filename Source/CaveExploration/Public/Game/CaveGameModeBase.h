// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CaveGameModeBase.generated.h"

class UAbilityInfo;
class UCharacterClassInfoDataAsset;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACaveGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfoDataAsset> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
};
