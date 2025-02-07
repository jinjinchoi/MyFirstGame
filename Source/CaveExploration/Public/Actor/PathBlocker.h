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
	
	void ActivateBlocker() const;
	void DeactivateBlocker() const;


protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;

private:
	
	

	
};
