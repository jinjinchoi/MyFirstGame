// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfoDataAsset.h"
#include "CaveCharacterBase.generated.h"

class UDebuffNiagaraComponent;
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
	virtual USoundBase* GetHitImpactSound_Implementation() override;
	virtual FOnASCRegistered GetOnASCRegisteredDelegate() override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	/* end Combat Interface */

	FOnASCRegistered OnAscRegistered;
	FOnDeath OnDeath;
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// TODO: 클라이언트에서 나이아가라 컴포넌트 전부 활성화 되는지 확인할 필요가 있음.
	UPROPERTY(ReplicatedUsing="OnRep_Stunned", BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing="OnRep_Burned", BlueprintReadOnly)
	bool bIsBurned = false;

	UPROPERTY(ReplicatedUsing="OnRep_Frozen", BlueprintReadOnly)
	bool bIsFrozen = false;


protected:
	/* Engine */
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	/* Engine */

	virtual void InitAbilityActorInfo();
	virtual void ReactGameplayTagChanged();
	virtual void HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount);
	void FrozenTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	void BurnedTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	UFUNCTION()
	virtual void OnRep_Frozen();
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bIsDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Information")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 350.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> FrozenDebuffComponent;

	
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

	UPROPERTY(EditAnywhere, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartupInteractionAbilities;

	UPROPERTY(EditAnywhere, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	virtual void InitializeDefaultAttributes() const;
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const;
	void AddCharacterAbilities() const;

#pragma endregion


private:
	UPROPERTY(EditAnywhere, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponTipSocketName = FName("WeaponTipSocket");

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<UAnimMontage*> HitReactMontages;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<UAnimMontage*> DeathMontages;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TObjectPtr<USoundBase> HitImpactSound;
	
};

