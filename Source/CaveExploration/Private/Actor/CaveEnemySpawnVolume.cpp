// Copyright Jonwoo-Choi


#include "Actor/CaveEnemySpawnVolume.h"

#include "Actor/CaveEnemySpawnPoint.h"
#include "Actor/PathBlocker.h"
#include "Components/BoxComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"


ACaveEnemySpawnVolume::ACaveEnemySpawnVolume()
{

	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}


void ACaveEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACaveEnemySpawnVolume::OnSphereOverlap);
	
}

void ACaveEnemySpawnVolume::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player") || HasSpawned) return;

	for (const APathBlocker* Blocker : BlockingWall)
	{
		if (IsValid(Blocker))
		{
			Blocker->ActivateEffect();
		}
	}

	for (ACaveEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			AActor* SpawnedEnemy = Point->SpawnEnemy();
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(SpawnedEnemy))
			{
				++NumOfSpawnedEnemy;
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ACaveEnemySpawnVolume::OnSpawnedEnemyDeath);
			}
		}
	}
	HasSpawned = true;
	Box->OnComponentBeginOverlap.RemoveDynamic(this, &ACaveEnemySpawnVolume::OnSphereOverlap);
}

void ACaveEnemySpawnVolume::OnSpawnedEnemyDeath(AActor* DeathEnemy)
{
	--NumOfSpawnedEnemy;
	if (NumOfSpawnedEnemy <= 0)
	{
		for (APathBlocker* Blocker : BlockingWall)
		{
			if (IsValid(Blocker))
			{
				Blocker->DeactivateEffect();
				Blocker->Destroy();
			}
		}
		if (IsBossSpawner)
		{
			HandleDungeonClear();
			FTimerHandle TimerHandle_LevelTransition;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_LevelTransition, this, &ACaveEnemySpawnVolume::ChangeLevel, 10.0f, false);
		}
		else
		{
			Destroy();
		}
	}
}

void ACaveEnemySpawnVolume::HandleDungeonClear() const
{
	if (DungeonID == FName())
	{
		UE_LOG(LogTemp, Warning, TEXT("DungeonID is Empty"));
		return;
	}
	
	TArray<AActor*> ExistingPlayers;
	UGameplayStatics::GetAllActorsWithTag(this, FName("Player"), ExistingPlayers);

	for (AActor* Player : ExistingPlayers)
	{
		if (!Player->HasAuthority()) continue;
		
		if (Player->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddClearedDungeon(Player, DungeonID);
			IPlayerInterface::Execute_SaveProgress(Player, CheckPointNameToGo, FString(""));

		}
	}
}

void ACaveEnemySpawnVolume::ChangeLevel() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, NextLevel);
}

