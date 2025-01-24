// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MagicCircle.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CAVEEXPLORATION_API AMagicCircle : public APawn
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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	
};
