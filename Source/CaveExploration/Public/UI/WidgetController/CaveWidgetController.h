// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "CaveWidgetController.generated.h"

class UCaveAttributeSet;
class UCaveAbilitySystemComponent;
class ACavePlayerState;
class ACavePlayerController;
class UAttributeSet;
class UAbilitySystemComponent;
class UAbilityInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoDelegate, const FCaveAbilityInfo&, Info);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController), PlayerState(InPlayerState), AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
};

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadCastInitialValues();
	
	virtual void BindCallbacksToDependencies();

	void BroadCastAbilityInfo();

	FAbilityInfoDelegate AbilityInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	ACavePlayerController* GetCavePlayerController();
	ACavePlayerState* GetCavePlayerState();
	UCaveAbilitySystemComponent* GetCaveAbilitySystemComponent();
	UCaveAttributeSet* GetCaveAttributeSet();

private:
	UPROPERTY()
	TObjectPtr<ACavePlayerController> CavePlayerController;

	UPROPERTY()
	TObjectPtr<ACavePlayerState> CavePlayerState;

	UPROPERTY()
	TObjectPtr<UCaveAbilitySystemComponent> CaveAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCaveAttributeSet> CaveAttributeSet;
	
};
