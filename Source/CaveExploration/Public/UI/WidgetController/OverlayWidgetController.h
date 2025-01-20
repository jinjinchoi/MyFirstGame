// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/CaveWidgetController.h"
#include "OverlayWidgetController.generated.h"



/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CAVEEXPLORATION_API UOverlayWidgetController : public UCaveWidgetController
{
	GENERATED_BODY()

public:
	/* Cave Widget Controller */
	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedDelegate OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedDelegate OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedDelegate OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedDelegate OnXPPercentChangedDelegate;
	/* end Cave Widget Controller */

protected:

	void OnXPChanged(int32 NewXP);
	
};
