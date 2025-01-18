// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "CaveAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "CaveProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class CAVEEXPLORATION_API ACaveProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACaveProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	UPROPERTY()
	FDamageEffectParams DamageEffectParams;
	

protected:
	/* Engine */
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	/* end Engine */

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	bool bHit = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
	float LifeSpan = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
	TObjectPtr<USoundBase> ImpactSound;

	void OnHit();
	bool IsValidOverlap(AActor* OtherActor) const;
};

