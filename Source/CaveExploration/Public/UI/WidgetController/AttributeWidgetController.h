// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/CaveWidgetController.h"
#include "AttributeWidgetController.generated.h"

class UAttributeInfoDataAsset;
struct FGameplayTag;
struct FCaveAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegate, const FCaveAttributeInfo&, Info);

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UAttributeWidgetController : public UCaveWidgetController
{
	GENERATED_BODY()

public:
	/* Cave Widget Controller */
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadCastInitialValues() override;
	/* end Cave Widget Controller */

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoDelegate AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerStateChangeDelegate AttributeChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfoDataAsset> AttributeInfo;

private:
	void BroadCastAttributeInfo(const FGameplayTag& AttributeTag) const;
};


