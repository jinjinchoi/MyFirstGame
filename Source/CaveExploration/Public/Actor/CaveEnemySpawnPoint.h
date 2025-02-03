// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfoDataAsset.h"
#include "Engine/TargetPoint.h"
#include "CaveEnemySpawnPoint.generated.h"

class ACaveEnemy;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACaveEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	AActor* SpawnEnemy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Class")
	TSubclassOf<ACaveEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Class")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	
};
