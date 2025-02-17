// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "CaveCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "CavePlayerCharacter.generated.h"

class UCaveAttributeSet;
class UNiagaraComponent;
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

	/* Player Interface */
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 InLevel) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 InLevel) const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToPlayerLevel_Implementation(int32 InLevel) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void LevelUp_Implementation() override;
	virtual void InPlayerComboAttack_Implementation() override;
	virtual void EndPlayerComboAttack_Implementation() override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual FVector GetMagicCircleLocation_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckPointTag, const FString& CheckPointName) override;
	virtual void AddClearedDungeon_Implementation(const FName& DungeonID) override;
	virtual bool IsDungeonCleared_Implementation(const FName& DungeonID) const override;
	virtual FVector GetCharacterMoveDirection_Implementation() const override;
	virtual void SetCharacterMoveDirection_Implementation(const FVector& NewDirection) override;
	/* end Player Interface */

protected:
	/* Engine */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	/* Engine */
	
	/* Cave Character Base */
	virtual void InitAbilityActorInfo() override;
	virtual void ReactGameplayTagChanged() override;
	virtual void HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount) override;
	virtual void DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount) override;
	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;
	virtual void OnRep_Frozen() override;
	/* end CaveCharacterBase */

	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	FName RespawnWorldNameForMultiPlay = FName("MainMenu");
	
	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float DeathTime = 5.f;

	FTimerHandle DeathTimer;
	
	void LoadProgress();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

private:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float BaseMaxWalkSpeed = 600.f;

	FRotator BaseRotationRate = FRotator(0, 540, 0);

	UPROPERTY(Replicated)
	FVector MoveDirection;
	
	UCaveAttributeSet* GetCaveAttributeSet() const;

	UFUNCTION(Server, Reliable)
	void ServerSetMoveDirection(const FVector& NewMoveDirection);


# pragma region Components
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

# pragma endregion Components	
};




