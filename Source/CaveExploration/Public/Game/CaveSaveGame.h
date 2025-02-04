// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "CaveSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Class Default")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Class Default", meta=(Categories = "Abilities.Spell"))
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Class Default", meta=(Categories = "Abilities.Status"))
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Class Default", meta=(Categories = "InputTag"))
	FGameplayTag AbilityInputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Class Default", meta=(Categories = "Abilities.Type"))
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Class Default")
	int32 AbilityLevel = 0;
	
};

inline bool operator==(const FSavedAbility& lhs, const FSavedAbility& rhs)
{
	return lhs.AbilityTag.MatchesTagExact(rhs.AbilityTag);
}

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus = Vacant;

	UPROPERTY()
	FString SavedDate = FString();

	/* 로드 슬롯에서 보이는 이름 */
	UPROPERTY()
	FString MapName = FString("Default Map Name");

	/* 실제 캐릭터가 존재하는 월드(레벨)의 경로 */
	UPROPERTY()
	FString MapAssetName = FString();
	
	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	bool bFirstTimeLoadIn = true;

	/* Player Status */

	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 SpellPoints = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;
	
	UPROPERTY()
	float Strength = 0;
	
	UPROPERTY()
	float Intelligence = 0;
	
	UPROPERTY()
	float Dexterity = 0;

	UPROPERTY()
	float Vigor = 0;

	UPROPERTY()
	TArray<FSavedAbility> SaveAbilities;

	UPROPERTY()
	TArray<FName> ClearedDungeons;
	
	
};
