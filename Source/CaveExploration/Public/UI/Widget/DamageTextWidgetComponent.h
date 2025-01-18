// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidgetComponent.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bCriticalHit, const FGameplayTag& DamageType);
	
};
