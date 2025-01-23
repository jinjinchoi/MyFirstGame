// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CaveDamageGameplayAbility.h"
#include "CaveBladeWave.generated.h"

class ACaveFireBlade;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveBladeWave : public UCaveDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Blade Properties")
	TSubclassOf<ACaveFireBlade> BladeClass;

	UFUNCTION(BlueprintCallable)
	void SpawnWave() const;
	
};
