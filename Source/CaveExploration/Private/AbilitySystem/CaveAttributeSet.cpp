// Copyright Jonwoo-Choi


#include "AbilitySystem/CaveAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CaveAbilityTypes.h"
#include "CaveFunctionLibrary.h"
#include "CaveGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/CavePlayerController.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UCaveAttributeSet::UCaveAttributeSet()
{
	
}

void UCaveAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* Primary Attribute */
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	/* Secondary Attribute */
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	/* Vital Attribute */
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCaveAttributeSet, Mana, COND_None, REPNOTIFY_Always);

}

void UCaveAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourcePlayerController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (!Props.SourcePlayerController && Props.SourceAvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourcePlayerController = Pawn->GetController();
			}
		}
		if (Props.SourcePlayerController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourcePlayerController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetPlayerController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


void UCaveAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UCaveAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bRecoverHealth)
	{
		SetHealth(GetMaxHealth());
		bRecoverHealth = false;
	}

	if (Attribute == GetMaxManaAttribute() && bRecoverMana)
	{
		SetMana(GetMaxMana());
		bRecoverMana = false;
	}
}


void UCaveAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDebuffDamageAttribute())
	{
		HandleIncomingDebuffDamage(Props);
	}
	
}


void UCaveAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	if (Props.TargetASC->HasMatchingGameplayTag(FCaveGameplayTags::Get().Abilities_Common_Invincible))
	{
		return;
	}
	
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);

	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			if (Props.TargetCharacter->Implements<UCombatInterface>())
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FCaveGameplayTags::Get().Abilities_Common_Death);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			SendXPEvent(Props);
		}
		else if (!Props.TargetASC->HasMatchingGameplayTag(FCaveGameplayTags::Get().Abilities_Common_SuperArmor))
		{
			if (Props.TargetCharacter->Implements<UCombatInterface>())
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FCaveGameplayTags::Get().Abilities_Common_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool IsKnockback = UCaveFunctionLibrary::IsKnockback(Props.EffectContextHandle);
			if (IsKnockback)
			{
				const FVector& KnockbackForce = UCaveFunctionLibrary::GetKnockbackDirection(Props.EffectContextHandle);
				Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}
		const bool bCriticalHit = UCaveFunctionLibrary::IsCriticalHit(Props.EffectContextHandle);
		const FGameplayTag& DamageType = UCaveFunctionLibrary::GetDamageType(Props.EffectContextHandle);
		
		ShowFloatDamage(Props, LocalIncomingDamage, bCriticalHit, DamageType);

		const bool IsSuccessfulDebuff = UCaveFunctionLibrary::IsSuccessfulDebuff(Props.EffectContextHandle);
		if (IsSuccessfulDebuff && !Props.TargetASC->HasMatchingGameplayTag(FCaveGameplayTags::Get().Abilities_Common_Immunity))
		{
			Debuff(Props);
		}
	}
}


void UCaveAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FCaveGameplayTags GameplayTags = FCaveGameplayTags::Get();

	FGameplayEffectContextHandle EffectContextHandle = Props.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UCaveFunctionLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UCaveFunctionLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UCaveFunctionLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UCaveFunctionLibrary::GetDebuffFrequncy(Props.EffectContextHandle);

	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	GameplayEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	GameplayEffect->Period = DebuffFrequency;
	GameplayEffect->DurationMagnitude = FScalableFloat(DebuffDuration);

	const FGameplayTag& DebuffTag = GameplayTags.DamageTypesToDebuff[DamageType];
	
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = GameplayEffect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(DebuffTag);

	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Type_Stun))
	{
		TagContainer.AddTag(GameplayTags.Player_Block_Released);
		TagContainer.AddTag(GameplayTags.Player_Block_InputHeld);
		TagContainer.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainer.AddTag(GameplayTags.Player_Block_Move);
	}

	Component.SetAndApplyTargetTagChanges(TagContainer);

	GameplayEffect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	GameplayEffect->StackLimitCount = 1;

	const int32 Index = GameplayEffect->Modifiers.Num();
	GameplayEffect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = GameplayEffect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDebuffDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(GameplayEffect, EffectContextHandle, 1.f))
	{
		FCaveGameplayEffectContext* CaveContext = static_cast<FCaveGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		CaveContext->SetDamageType(DebuffDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}	
}

void UCaveAttributeSet::HandleIncomingDebuffDamage(const FEffectProperties& Props)
{
	const float LocalDebuffDamage = GetIncomingDebuffDamage();
	SetIncomingDebuffDamage(0.f);
	if (LocalDebuffDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalDebuffDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			if (Props.TargetCharacter->Implements<UCombatInterface>())
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FCaveGameplayTags::Get().Abilities_Common_Death);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			SendXPEvent(Props);
		}
		const bool bCriticalHit = UCaveFunctionLibrary::IsCriticalHit(Props.EffectContextHandle);
		const FGameplayTag& DamageType = UCaveFunctionLibrary::GetDamageType(Props.EffectContextHandle);
		ShowFloatDamage(Props, LocalDebuffDamage, bCriticalHit, DamageType);
	}
	
}

void UCaveAttributeSet::SendXPEvent(const FEffectProperties& Props) const
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetCharacterLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UCaveFunctionLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FCaveGameplayTags& GameplayTag = FCaveGameplayTags::Get();
		
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTag.Attribute_Meta_InComingXP;
		Payload.EventMagnitude = XPReward;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTag.Attribute_Meta_InComingXP, Payload);
	}
}

void UCaveAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;
			int32 ChangedLevel = CurrentLevel;
			for (int32 i = 0; i < NumLevelUps; ++i)
			{
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, ChangedLevel);
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, ChangedLevel);
				++ChangedLevel;
			}

			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
			
			bRecoverHealth = true;
			bRecoverMana = true;
		}

		
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UCaveAttributeSet::ShowFloatDamage(const FEffectProperties& Props, const float Damage, const bool bIsCriticalHit, const FGameplayTag& DamageType) const
{
	if (Props.SourceCharacter == Props.TargetCharacter) return;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ACavePlayerController* PC = Cast<ACavePlayerController>(It->Get()))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bIsCriticalHit, DamageType);
		}
	}
}




void UCaveAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, Strength, OldStrength);
}

void UCaveAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, Intelligence, OldIntelligence);
}

void UCaveAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, Dexterity, OldDexterity);
}

void UCaveAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, Vigor, OldVigor);
}

void UCaveAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, Armor, OldArmor);
}

void UCaveAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UCaveAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UCaveAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UCaveAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UCaveAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UCaveAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, MaxHealth, OldMaxHealth);
}

void UCaveAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, MaxMana, OldMaxMana);
}

void UCaveAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, FireResistance, OldFireResistance);
}

void UCaveAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, IceResistance, OldIceResistance);
}

void UCaveAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UCaveAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, Health, OldHealth);
}

void UCaveAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCaveAttributeSet, Mana, OldMana);
}

