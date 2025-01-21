// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/CaveWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API USpellMenuWidgetController : public UCaveWidgetController
{
	GENERATED_BODY()

public:
	/* Cave Widget Controller */
	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	/* end Cave Widget Controller */


	
};
