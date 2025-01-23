// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "CaveAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "CaveFireBlade.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class CAVEEXPLORATION_API ACaveFireBlade : public AActor
{
	GENERATED_BODY()
	
public:	
	ACaveFireBlade();

	/* Engine */
	virtual void Destroyed() override;
	/* end Engine */

	UFUNCTION(BlueprintCallable)
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	FDamageEffectParams DamageEffectParams;

protected:
	/* Engine */
	virtual void BeginPlay() override;
	/* end Engine */
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category="Blade Properties")
	float LifeSpan = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Blade Properties")
	float DamageInterval = 0.3f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
	TObjectPtr<USoundBase> ImpactSound;

	bool IsValidOverlap(AActor* OtherActor) const;

	FTimerHandle OverlapTimerHandle;

	UPROPERTY()
	TArray<AActor*> OverlappingActors;

	void ApplyGameplayEffect();

	UFUNCTION(Client, Unreliable)
	void OnHit();
	
};

