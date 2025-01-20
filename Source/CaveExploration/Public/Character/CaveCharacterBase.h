// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfoDataAsset.h"
#include "CaveCharacterBase.generated.h"

struct FGameplayTag;
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
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetDeathMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	/* end Combat Interface */
	
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	/* Engine */
	virtual void BeginPlay() override;
	/* Engine */

	virtual void InitAbilityActorInfo();
	virtual void HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bIsDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Information")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	
#pragma region Attribute And Ability
	
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
	
	void AddCharacterAbilities();

#pragma endregion


private:
	UPROPERTY(EditAnywhere, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponTipSocketName = FName("WeaponTipSocket");

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<UAnimMontage*> HitReactMontages;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<UAnimMontage*> DeathMontages;
	
	
};
