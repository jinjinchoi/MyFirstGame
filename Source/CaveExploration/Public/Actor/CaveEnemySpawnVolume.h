// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaveEnemySpawnVolume.generated.h"

class APathBlocker;
class UBoxComponent;
class ACaveEnemySpawnPoint;

UCLASS()
class CAVEEXPLORATION_API ACaveEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ACaveEnemySpawnVolume();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	FName DungeonID;

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	FName CheckPointNameToGo;

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	float TimeForLevelChange = 8.f;
	

private:
	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	TArray<ACaveEnemySpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	TArray<APathBlocker*> BlockingWall;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> Box;
	
	int32 NumOfSpawnedEnemy = 0;
	bool HasSpawned = false;

	UFUNCTION()
	void OnSpawnedEnemyDeath(AActor* DeathActor);
	
	void HandleDungeonClear() const;

	
	/* Handle Level Change*/

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	bool IsBossSpawner = false;

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	TSoftObjectPtr<UWorld> NextLevel;
	
	void ChangeLevel() const;

	

};
