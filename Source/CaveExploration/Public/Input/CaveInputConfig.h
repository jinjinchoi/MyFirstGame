// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CaveInputConfig.generated.h"


class UInputAction;

USTRUCT(BlueprintType)
struct FCaveInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(Categories = "InputTag"))
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCaveInputAction> InputActions;
};
