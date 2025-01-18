// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfoDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FCaveAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Attribute"))
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute AttributeGetter;
	
};

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UAttributeInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FCaveAttributeInfo FindAttributeInfoForTag(const FGameplayTag& InAttributeTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "{AttributeName}"))
	TArray<FCaveAttributeInfo> AttributeInformation;
};
