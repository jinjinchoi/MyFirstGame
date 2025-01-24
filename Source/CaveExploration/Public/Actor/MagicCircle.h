// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CAVEEXPLORATION_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();
	
	void Move(const FVector2D& Value);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed = 600.0f;

	UFUNCTION(Server, Reliable)
	void ServerMove(const FVector& InMovement);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastMove(FVector NewLocation);


	
};


