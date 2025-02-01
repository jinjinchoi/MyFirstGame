// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CaveSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	Taken
};

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

	UPROPERTY()
	FString MapName = FString("Default Map Name");
	
	
};
