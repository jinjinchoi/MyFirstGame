// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "CaveCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "CaveEnemy.generated.h"

class ACaveAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACaveEnemy : public ACaveCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ACaveEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	/* Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override;
	/* end Combat Interface */

	/* Overlay Widget Controller */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedDelegate OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedDelegate OnMaxHealthChangedDelegate;
	/* end Overlay Widget Controller */

protected:
	/* Engine */
	virtual void BeginPlay() override;
	/* end Engine */

	/* Cave Character Base */
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void ReactGameplayTagChanged() override;
	virtual void HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount) override;
	virtual void DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount) override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	/* end Cave Character Base */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Information")
	int32 EnemyLevel = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;

	UPROPERTY(EditDefaultsOnly, Category=AI)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ACaveAIController> CaveAIController;
	
};
