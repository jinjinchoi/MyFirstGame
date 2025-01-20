// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CaveFunctionLibrary.generated.h"

class UAbilitySystemComponent;
struct FDamageEffectParams;
struct FGameplayEffectContextHandle;
class UAttributeWidgetController;
class ACaveHUD;
struct FWidgetControllerParams;
class UOverlayWidgetController;
enum class ECharacterClass : uint8;

/**
 * 
 */

UCLASS()
class CAVEEXPLORATION_API UCaveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#pragma region  Widget
	
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParam, ACaveHUD*& OutCaveHUD);
	
	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static  UAttributeWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);

#pragma endregion

	
#pragma region Effect Context

	/**
	 * Getter
	 */

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|GameplayEffects")
	static float GetDebuffFrequncy(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|GameplayEffects")
	static bool IsKnockback(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="CaveFunctionLibrary|GameplayEffects")
	static FVector GetKnockbackDirection(const FGameplayEffectContextHandle& EffectContextHandle);

	
	/**
	 * Setter
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,  const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsKnockback(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsKnockback);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackDirection(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackDirection);

#pragma endregion


#pragma region  Gameplay Mechanics

	UFUNCTION(BlueprintCallable, Category="CaveFunctionLibrary|GameplayMechanics")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category="CaveFunctionLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* A, AActor* B);

	UFUNCTION(BlueprintCallable, Category="CaveFunctionLibrary|GameplayMechanics")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>&ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category="CaveFunctionLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpaceRotators(const FVector& Forward, float Spread, int32 NumOfRotators);
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category="CaveFunctionLibrary|GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, float Spread, int32 NumOfVectors);

	

#pragma endregion

#pragma region AbilitySystem
	
	UFUNCTION(BlueprintCallable, Category="CaveFunctionLibrary|CharacterClassDefaults")
	static UCharacterClassInfoDataAsset* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="CaveFunctionLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttribute(const UObject* WorldContextObject, const ECharacterClass& CharacterClass, const float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="CaveFunctionLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const int32 CharacterLevel);

#pragma endregion
};


