// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "CaveCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "CavePlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACavePlayerCharacter : public ACaveCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	ACavePlayerCharacter();
	
	/* Engine */
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/* end Engine */

	/* Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override;
	/* end Combat Interface */
	


protected:
	/* Cave Character Base */
	virtual void InitAbilityActorInfo() override;
	/* end CaveCharacterBase */

private:

# pragma region Components
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

# pragma endregion Components	
};
