// Copyright Jonwoo-Choi


#include "CaveFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CaveAbilityTypes.h"
#include "CaveGameplayTags.h"
#include "NiagaraValidationRule.h"
#include "AbilitySystem/Abilities/CaveGameplayAbility.h"
#include "AbilitySystem/Data/CharacterClassInfoDataAsset.h"
#include "Engine/OverlapResult.h"
#include "Game/CaveGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CavePlayerState.h"
#include "UI/HUD/CaveHUD.h"
#include "UI/WidgetController/CaveWidgetController.h"

bool UCaveFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParam, ACaveHUD*& OutCaveHUD)
{
	if (APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		OutCaveHUD = Cast<ACaveHUD>(PlayerController->GetHUD());
		if (OutCaveHUD)
		{
			ACavePlayerState* PlayerState = PlayerController->GetPlayerState<ACavePlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			OutWCParam.AttributeSet = AttributeSet;
			OutWCParam.AbilitySystemComponent = AbilitySystemComponent;
			OutWCParam.PlayerController = PlayerController;
			OutWCParam.PlayerState = PlayerState;

			return true;
		}
	}

	return false;
}

UOverlayWidgetController* UCaveFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParam;
	ACaveHUD* CaveHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParam, CaveHUD))
	{
		return CaveHUD->GetOverlayWidgetController(WCParam);
	}

	return nullptr;
}

UAttributeWidgetController* UCaveFunctionLibrary::GetAttributeWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParam;
	ACaveHUD* CaveHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParam, CaveHUD))
	{
		return CaveHUD->GetAttributeWidgetController(WCParam);
	}

	return nullptr;
}

USpellMenuWidgetController* UCaveFunctionLibrary::GetSpellMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParam;
	ACaveHUD* CaveHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParam, CaveHUD))
	{
		return CaveHUD->GetSpellMenuController(WCParam);
	}

	return nullptr;
}

FString UCaveFunctionLibrary::GetAbilityDescription(const UObject* WorldContextObject, const FGameplayTag& AbilityTag, int32 AbilityLevel, const FCaveAbilityInfo& AbilityInfo)
{
	if (!AbilityInfo.AbilityTag.IsValid()) return FString();
	if (AbilityLevel <= 0) AbilityLevel = 1;
	
	UCaveGameplayAbility* Ability = Cast<UCaveGameplayAbility>(AbilityInfo.AbilityClass.GetDefaultObject());
	
	FText FormatedText = FormatAbilityDescription(Ability, AbilityLevel, AbilityInfo);
	
	return FString::Printf(
		TEXT(
		// 스킬명 및 레벨
		"<Title>%s</>\n"
		"<Level>Lv.%d</>\n"

		// 스킬 설명
		"\n"
		"%s\n"
		"\n"

		// 마나 및 쿨다운 정보
		"마나: <Mana>%.1f</>\n"
		"쿨타임: <Cooldown>%.1f</>초\n"
		),
		*AbilityInfo.AbilityName,
		AbilityLevel,
		*FormatedText.ToString(),
		Ability->GetManaCost(AbilityLevel),
		Ability->GetCoolDown(AbilityLevel)
	);
}

FText UCaveFunctionLibrary::FormatAbilityDescription(const UCaveGameplayAbility* Ability, const int32 AbilityLevel, const FCaveAbilityInfo& AbilityInfo)
{
	
	FText AbilityDescription = AbilityInfo.Description;
	AbilityDescription = FText::FormatNamed(
		AbilityDescription,
		TEXT("_Damage"),
		FText::AsNumber(Ability->GetDamage(AbilityLevel))
	);

	return AbilityDescription;
}

bool UCaveFunctionLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCaveGameplayEffectContext* CaveContext = static_cast<const FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CaveContext->IsCriticalHit();
	}

	return false;
}

bool UCaveFunctionLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCaveGameplayEffectContext* CaveContext = static_cast<const FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CaveContext->IsSuccessfulDebuff();
	}

	return false;
}

float UCaveFunctionLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCaveGameplayEffectContext* CaveContext = static_cast<const FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CaveContext->GetDebuffDamage();
	}

	return 0;
}

float UCaveFunctionLibrary::GetDebuffFrequncy(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCaveGameplayEffectContext* CaveContext = static_cast<const FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CaveContext->GetDebuffFrequncy();
	}

	return 0;
}

float UCaveFunctionLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCaveGameplayEffectContext* CaveContext = static_cast<const FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CaveContext->GetDebuffDuration();
	}

	return 0;
}

FGameplayTag UCaveFunctionLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCaveGameplayEffectContext* CaveContext = static_cast<const FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (CaveContext->GetDamageType().IsValid())
		{
			return *CaveContext->GetDamageType();
		}
	}

	return FGameplayTag();
}

bool UCaveFunctionLibrary::IsKnockback(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCaveGameplayEffectContext* CaveContext = static_cast<const FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CaveContext->IsKnockback();
	}

	return false;
}

FVector UCaveFunctionLibrary::GetKnockbackDirection(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCaveGameplayEffectContext* CaveContext = static_cast<const FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CaveContext->GetKnockbackDirection();
	}

	return FVector::ZeroVector;
}

void UCaveFunctionLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit)
{
	if (FCaveGameplayEffectContext* CaveContext = static_cast<FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CaveContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UCaveFunctionLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsSuccessfulDebuff)
{
	if (FCaveGameplayEffectContext* CaveContext = static_cast<FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CaveContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UCaveFunctionLibrary::SetIsDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffDamage)
{
	if (FCaveGameplayEffectContext* CaveContext = static_cast<FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CaveContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UCaveFunctionLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffDuration)
{
	if (FCaveGameplayEffectContext* CaveContext = static_cast<FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CaveContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UCaveFunctionLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FCaveGameplayEffectContext* CaveContext = static_cast<FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		CaveContext->SetDamageType(DamageType);
	}
}

void UCaveFunctionLibrary::SetIsKnockback(FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsKnockback)
{
	if (FCaveGameplayEffectContext* CaveContext = static_cast<FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CaveContext->SetIsKnockback(bInIsKnockback);
	}
}

void UCaveFunctionLibrary::SetKnockbackDirection(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackDirection)
{
	if (FCaveGameplayEffectContext* CaveContext = static_cast<FCaveGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CaveContext->SetKnockBackDirection(InKnockbackDirection);
	}
}

FGameplayEffectContextHandle UCaveFunctionLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();

	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	FGameplayEffectContextHandle ContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceAvatarActor);

	if (DamageEffectParams.bIsKnockback)
	{
		SetKnockbackDirection(ContextHandle, DamageEffectParams.KnockbackDirection);
	}

	if (DamageEffectParams.DamageType.IsValid())
	{
		SetDamageType(ContextHandle, DamageEffectParams.DamageType);
	}

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Properties_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Properties_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Properties_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Properties_Frequency, DamageEffectParams.DebuffFrequncy);

	DamageEffectParams.TartgetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	return ContextHandle;
}

bool UCaveFunctionLibrary::IsNotFriend(AActor* A, AActor* B)
{
	const bool bBothArePlayers = A->ActorHasTag(FName("Player")) && B->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = A->ActorHasTag(FName("Enemy")) && B->ActorHasTag(FName("Enemy"));
	const bool bFreinds = bBothArePlayers || bBothAreEnemies;
	return !bFreinds;
}

void UCaveFunctionLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> OverlapResults;

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(
			OverlapResults,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);

	
		for (FOverlapResult& Overlap : OverlapResults)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
		
	}
}

TArray<FRotator> UCaveFunctionLibrary::EvenlySpaceRotators(const FVector& Forward, float Spread, int32 NumOfRotators)
{
	TArray<FRotator> Rotators;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, FVector::UpVector);
	if (NumOfRotators > 1)
	{
		const float DeltaSpread = Spread / (NumOfRotators - 1);
		
		for (int32 i = 0; i < NumOfRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	
	return Rotators;
}

TArray<FVector> UCaveFunctionLibrary::EvenlyRotatedVectors(const FVector& Forward, float Spread, int32 NumOfVectors)
{
	TArray<FVector> Vectors;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, FVector::UpVector);
	if (NumOfVectors > 1)
	{
		const float DeltaSpread = Spread / (NumOfVectors - 1);
		
		for (int32 i = 0; i < NumOfVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	
	return Vectors;
}



UCharacterClassInfoDataAsset* UCaveFunctionLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ACaveGameModeBase* CaveGameMode =  Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (CaveGameMode)
	{
		return CaveGameMode->CharacterClassInfo;
	}
	else
	{
		return nullptr;
	}
}

UAbilityInfo* UCaveFunctionLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ACaveGameModeBase* CaveGameMode =  Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (CaveGameMode)
	{
		return CaveGameMode->AbilityInfo;
	}
	else
	{
		return nullptr;
	}
}

void UCaveFunctionLibrary::InitializeDefaultAttribute(const UObject* WorldContextObject, const ECharacterClass& CharacterClass, const float Level, UAbilitySystemComponent* ASC)
{
	const UCharacterClassInfoDataAsset* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);

	const AActor* AvatarActor = ASC->GetAvatarActor();
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle ResistanceSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.ResistanceAttributes, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*ResistanceSpecHandle.Data.Get());
	
}

void UCaveFunctionLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	const UCharacterClassInfoDataAsset* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	//TODO : 추가해야함
}

int32 UCaveFunctionLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const int32 CharacterLevel)
{
	const UCharacterClassInfoDataAsset* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
	
}

TArray<FGameplayTag> UCaveFunctionLibrary::CallerMagnitudeTags(TSubclassOf<UGameplayEffect> InGameplayEffect)
{
	TArray<FGameplayTag> ResultTag;
	
	if (!IsValid(InGameplayEffect)) return ResultTag;

	UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), InGameplayEffect);

	if (!GameplayEffect) return ResultTag;

	// Gameplay Effect의 Modifier 들을 순회 하여 ModifierMagnitude 계산 방식이 SetByCaller 인지 확인하고 맞으면 데이터 태그를 ResultTag 배열에 추가
	for (const FGameplayModifierInfo& ModifierInfo : GameplayEffect->Modifiers)
	{
		if (ModifierInfo.ModifierMagnitude.GetMagnitudeCalculationType() == EGameplayEffectMagnitudeCalculation::SetByCaller)
		{
			ResultTag.Add(ModifierInfo.ModifierMagnitude.GetSetByCallerFloat().DataTag);
		}
	}

	return ResultTag;
}


