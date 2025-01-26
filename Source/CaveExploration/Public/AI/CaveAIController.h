// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CaveAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACaveAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACaveAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
