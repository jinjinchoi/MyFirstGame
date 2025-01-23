#pragma once

#include "GameplayEffectTypes.h"
#include "CaveAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TartgetAbilitySystemComponent = nullptr;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffFrequncy = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	bool bIsKnockback = false;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FVector KnockbackDirection = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackForceMafnitude = 1000.f;
	
};

USTRUCT(BlueprintType)
struct FCaveGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FCaveGameplayEffectContext* Duplicate() const override
	{
		FCaveGameplayEffectContext* NewContext = new FCaveGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	
#pragma region  Getter

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffFrequncy() const { return DebuffFrequncy; }
	float GetDebuffDuration() const { return DebuffDuration; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType;}
	bool IsKnockback() const { return bIsKnockback; }
	FVector GetKnockbackDirection() const { return KnockbackDirection; }

#pragma endregion
	
#pragma region Setter

	void SetIsCriticalHit(const bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsSuccessfulDebuff(const bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffDamage(const float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDebuffDuration(const float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequncy(const float InDebuffFrequncy) { DebuffFrequncy = InDebuffFrequncy; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
	void SetIsKnockback(const bool bInIsKockback) { bIsKnockback = bInIsKockback; }
	void SetKnockBackDirection(const FVector& InDirection ) { KnockbackDirection = InDirection; }

	#pragma endregion
	

protected:
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequncy = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	bool bIsKnockback = false;

	UPROPERTY()
	FVector KnockbackDirection = FVector::ZeroVector;
	
};



template<>
struct TStructOpsTypeTraits<FCaveGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FCaveGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
