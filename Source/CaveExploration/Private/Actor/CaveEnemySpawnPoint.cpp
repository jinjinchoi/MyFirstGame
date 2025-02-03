// Copyright Jonwoo-Choi


#include "Actor/CaveEnemySpawnPoint.h"

#include "Character/CaveEnemy.h"

AActor* ACaveEnemySpawnPoint::SpawnEnemy()
{
	check(EnemyClass);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACaveEnemy* Enemy = GetWorld()->SpawnActorDeferred<ACaveEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetEnemyLevel(EnemyLevel);
	Enemy->SetEnemyClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());

	return Enemy;
}
