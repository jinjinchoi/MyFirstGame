// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CaveAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {};
	
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourcePlayerController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetPlayerController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
};

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCaveAttributeSet();
	
	/* Engine */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	/* end Engine */



#pragma region Primary Attribute
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, Strength);

	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attribute")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, Intelligence);

	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Primary Attribute")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, Dexterity);

	UFUNCTION()
	virtual void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attribute")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, Vigor);

	UFUNCTION()
	virtual void OnRep_Vigor(const FGameplayAttributeData& OldVigor);

#pragma endregion

#pragma region Secondary Attribute

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attribute")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, Armor);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attribute")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, ArmorPenetration);

	UFUNCTION()
	virtual void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, CriticalHitChance);

	UFUNCTION()
	virtual void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, CriticalHitDamage);

	UFUNCTION()
	virtual void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, HealthRegeneration);

	UFUNCTION()
	virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, ManaRegeneration);

	UFUNCTION()
	virtual void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, MaxHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, MaxMana);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

#pragma endregion

#pragma region Resistance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attribute")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, FireResistance);

	UFUNCTION()
	virtual void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IceResistance, Category = "Resistance Attribute")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, IceResistance);

	UFUNCTION()
	virtual void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attribute")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, PhysicalResistance);

	UFUNCTION()
	virtual void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance);

#pragma endregion 

#pragma region Vital Attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, Mana);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

#pragma endregion

#pragma region Meta Attributes
	
	UPROPERTY(BlueprintReadOnly, Category = "MetaAttribute")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "MetaAttribute")
	FGameplayAttributeData IncomingDebuffDamage;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, IncomingDebuffDamage);
	
	UPROPERTY(BlueprintReadOnly, Category = "MetaAttribute")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UCaveAttributeSet, IncomingXP);
	
#pragma endregion
	
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	void HandleIncomingDamage(const FEffectProperties& Props);
	void ShowFloatDamage(const FEffectProperties& Props, const float Damage, const bool bIsCriticalHit, const FGameplayTag& DamageType) const;
	void Debuff(const FEffectProperties& Props);
	void HandleIncomingDebuffDamage(const FEffectProperties& Props);
	void SendXPEvent(const FEffectProperties& Props) const;
	void HandleIncomingXP(const FEffectProperties& Props);

	bool bRecoverHealth = false;
	bool bRecoverMana = false;
};





