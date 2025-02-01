// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CheckPoint.generated.h"

class UNiagaraComponent;
class USphereComponent;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACheckPoint : public APlayerStart
{
	GENERATED_BODY()

public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
};
