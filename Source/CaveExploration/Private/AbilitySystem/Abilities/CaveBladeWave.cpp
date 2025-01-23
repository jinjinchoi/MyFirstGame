// Copyright Jonwoo-Choi


#include "AbilitySystem/Abilities/CaveBladeWave.h"

#include "Actor/CaveFireBlade.h"
#include "Interaction/CombatInterface.h"

void UCaveBladeWave::SpawnWave() const
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority() || !GetAvatarActorFromActorInfo()->Implements<UCombatInterface>()) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo());

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());

	ACaveFireBlade* Blade = GetWorld()->SpawnActorDeferred<ACaveFireBlade>(
		BladeClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	Blade->DamageEffectParams = MakeDamageEffectParams();
	Blade->SetOwner(GetAvatarActorFromActorInfo());	

	Blade->FinishSpawning(SpawnTransform);
	

}
