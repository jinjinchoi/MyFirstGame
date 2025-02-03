// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathBlocker.generated.h"

class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class CAVEEXPLORATION_API APathBlocker : public AActor
{
	GENERATED_BODY()
	
public:	
	APathBlocker();

	void ActivateEffect() const;
	void DeactivateEffect() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
	
};
