// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


struct FCaveGameplayTags
{

	static const FCaveGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

#pragma region Primary Attributes
	
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Dexterity;
	FGameplayTag Attribute_Primary_Vigor;
	
#pragma endregion

	
#pragma region Secondary Attributes
	
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_CriticalChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;

#pragma endregion

	
#pragma region Resistance
	
	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Ice;
	FGameplayTag Attribute_Resistance_Physical;
	
#pragma endregion

	
#pragma region MetaAttributes
	
	FGameplayTag Attribute_Meta_InComingXP;

#pragma endregion

	
#pragma region Damage Type

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Physical;


#pragma endregion

	
#pragma region Debuff

	FGameplayTag Debuff_Type_Burn;
	FGameplayTag Debuff_Type_Frozen;
	FGameplayTag Debuff_Type_Stun;

	FGameplayTag Debuff_Properties_Damage;
	FGameplayTag Debuff_Properties_Chance;
	FGameplayTag Debuff_Properties_Duration;
	FGameplayTag Debuff_Properties_Frequency;
	

#pragma endregion

	
#pragma region Abilities

	FGameplayTag Abilities_None;

	FGameplayTag Abilities_Common_Attack;
	FGameplayTag Abilities_Common_Summon;
	FGameplayTag Abilities_Common_Down;
	FGameplayTag Abilities_Common_HitReact;
	FGameplayTag Abilities_Common_Death;

	FGameplayTag Abilities_Status;
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	
	FGameplayTag Abilities_Type_None;
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;

	FGameplayTag Abilities_Spell_Fire_FireSlash;
	FGameplayTag Abilities_Spell_Physical_MeleeAttack;
	FGameplayTag Abilities_Spell_Physical_Thrust;
	FGameplayTag Abilities_Spell_Physical_Strike;
	FGameplayTag Abilities_Spell_Ice_FrozenShard;

#pragma endregion

	
#pragma region Montage And Socket

	FGameplayTag CombatSocket_1;
	FGameplayTag CombatSocket_2;
	FGameplayTag CombatSocket_3;
	FGameplayTag CombatSocket_4;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

#pragma endregion	

	
#pragma region Maps
	
	TMap<FGameplayTag /* DamageType */, FGameplayTag /* Resistance */> DamageTypesToResistance; 
	TMap<FGameplayTag /* DamageType */, FGameplayTag /* DebuffType */> DamageTypesToDebuff;
	TMap<FGameplayTag /* CombatSocket */ , FGameplayTag /* Montage */> CombatSocketToMontage;

#pragma endregion

	
#pragma region InputTag
	
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;
	FGameplayTag InputTag_A;
	FGameplayTag InputTag_S;
	FGameplayTag InputTag_Shift;

#pragma endregion
	

#pragma region BlockTag
	
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_Released;
	FGameplayTag Player_Block_InputHeld;
	
#pragma endregion
	
private:
	static  FCaveGameplayTags GameplayTags;
	
};