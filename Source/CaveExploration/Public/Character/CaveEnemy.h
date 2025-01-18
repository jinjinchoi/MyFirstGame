// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "CaveCharacterBase.h"
#include "AbilitySystem/Data/CharacterClassInfoDataAsset.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/CaveWidgetController.h"
#include "CaveEnemy.generated.h"

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
	/* end Cave Character Base */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Information")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Information")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
};
