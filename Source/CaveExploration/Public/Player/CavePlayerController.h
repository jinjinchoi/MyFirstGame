// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CavePlayerController.generated.h"

class UCaveAbilitySystemComponent;
struct FGameplayTag;
class UCaveInputConfig;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACavePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACavePlayerController();

protected:
	/* Engine */
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	/* end Engine */


private:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputMappingContext> CaveContext;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputAction> MoveAction;
	
	void Move(const FInputActionValue& InputValue);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UCaveInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UCaveAbilitySystemComponent> CaveAbilitySystemComponent;

	UCaveAbilitySystemComponent* GetASC();

	
	
};
