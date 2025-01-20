// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FCaveLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 10;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 2;
};

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="LevelUpInfo")
	TArray<FCaveLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(const int32 XP) const;
};
