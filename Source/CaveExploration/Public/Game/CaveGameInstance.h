// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CaveGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;

	UPROPERTY()
	bool bIsMultiplayer = false;
};
