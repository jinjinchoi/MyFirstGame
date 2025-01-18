// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemInterface.h"
#include "CaveCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class CAVEEXPLORATION_API ACaveCharacterBase : public ACharacter, public  IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACaveCharacterBase();

	/* AbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* AbilitySystemInterface  */

	/* Combat Interface */
	virtual FVector GetWeaponSocketLocation_Implementation() override;
	/* end Combat Interface */
	
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	/* Engine */
	virtual void BeginPlay() override;
	/* Engine */

	virtual void InitAbilityActorInfo();

	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

#pragma region Attributes
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	TSubclassOf<UGameplayEffect> PrimaryAttributesClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	TSubclassOf<UGameplayEffect> SecondaryAttributesClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	TSubclassOf<UGameplayEffect> VitalAttributesClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	TSubclassOf<UGameplayEffect> ResistanceClass;

	virtual void InitializeDefaultAttributes() const;
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const;
	
#pragma endregion

#pragma  region Abilities
	
	void AddCharacterAbilities();


#pragma endregion	

private:
	UPROPERTY(EditAnywhere, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponTipSocketName = FName("WeaponTipSocket");
	
	
};
