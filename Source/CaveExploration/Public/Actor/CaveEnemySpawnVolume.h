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
	/* Engine */
	virtual void BeginPlay() override;
	/* end Engine */

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*  Dungeon 클리어 후 저장할 현재 던전의 이름 */
	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	FName DungeonID;

	/* Dungeon 클리어 후 갈 레벨의 체크포인트 이름 */
	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	FName CheckPointNameToGo = FName("Square1Start1");

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	float TimeForLevelChange = 8.f;

	/* Dungeon 클리어 후 게임이 느려지는 동안의 시간 */
	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	float SlowTime = 2.f;

	/* Dungeon 클리어 후 게임이 느려질때 얼마나 느려지는지 비율 */
	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	float SlowRate = 0.25f;
	

private:
	
	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	TArray<ACaveEnemySpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	TArray<TObjectPtr<APathBlocker>> BlockingWall;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> Box;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> ClearMessageWidget;

	UPROPERTY(EditAnywhere, Category="Spawn Properties")
	TSubclassOf<UUserWidget> ClearMessageWidgetClass;
	
	int32 NumOfSpawnedEnemies = 0;
	
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
	void CreateDungeonClearMessageWidget();

	UFUNCTION(Server, Reliable)
	void SlowDownGame();

	void ResetGameSpeed();

	/* Network*/
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateTimeDilation(float NewTimeDilation);
	
	
};
