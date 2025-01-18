// Copyright Jonwoo-Choi


#include "CaveGameplayTags.h"
#include "GameplayTagsManager.h"

FCaveGameplayTags FCaveGameplayTags::GameplayTags;

void FCaveGameplayTags::InitializeNativeGameplayTags()
{

#pragma region  Primary Attributes
	
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Strength"));

	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Intelligence"));

	GameplayTags.Attribute_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Dexterity"));

	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Vigor"));

#pragma endregion


#pragma region  Secondary Attributes
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.Armor"));

	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ArmorPenetration"));

	GameplayTags.Attribute_Secondary_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalChance"));

	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitDamage"));

	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.HealthRegeneration"));

	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ManaRegeneration"));

	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxHealth"));

	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxMana"));

#pragma endregion


#pragma region Resistance
	
	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Resistance.Fire"));
	
	GameplayTags.Attribute_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Resistance.Ice"));

	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Resistance.Physical"));

#pragma endregion

	
#pragma region Damage Type
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"));

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"));

	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Ice"));

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"));
	
#pragma endregion

	
#pragma region Debuff

	GameplayTags.Debuff_Type_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Type.Burn"));

	GameplayTags.Debuff_Type_Frozen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Type.Frozen"));

	GameplayTags.Debuff_Type_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Type.Stun"));

	GameplayTags.Debuff_Properties_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Properties.Damage"));

	GameplayTags.Debuff_Properties_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Properties.Chance"));
	
	GameplayTags.Debuff_Properties_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Properties.Duration"));

	GameplayTags.Debuff_Properties_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Properties.Frequency"));


#pragma endregion

	
#pragma region Abilities
	
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"));

	GameplayTags.Abilities_Common_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Common.Attack"));

	GameplayTags.Abilities_Common_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Common.Summon"));

	GameplayTags.Abilities_Common_Down = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Common.Down"));
	
	GameplayTags.Abilities_Common_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Common.HitReact"));

	GameplayTags.Abilities_Common_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Common.Death"));

	
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"));
	
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"));

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"));

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"));

	
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"));

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"));

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"));

	
	GameplayTags.Abilities_Fire_FireSlash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireSlash"));

	GameplayTags.Abilities_Physical_MeleeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.MeleeAttack"));

#pragma endregion

	
#pragma region  Montage and socket

	GameplayTags.CombatSocket_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.1"));

	GameplayTags.CombatSocket_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.2"));

	GameplayTags.CombatSocket_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.3"));

	GameplayTags.CombatSocket_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.4"));

	
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"));

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"));

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"));

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"));

#pragma endregion

	
#pragma region Maps
	
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire, GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Ice, GameplayTags.Attribute_Resistance_Ice);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attribute_Resistance_Physical);

	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Type_Burn);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Ice, GameplayTags.Debuff_Type_Frozen);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Type_Stun);

	GameplayTags.CombatSocketToMontage.Add(GameplayTags.CombatSocket_1, GameplayTags.Montage_Attack_1);
	GameplayTags.CombatSocketToMontage.Add(GameplayTags.CombatSocket_2, GameplayTags.Montage_Attack_2);
	GameplayTags.CombatSocketToMontage.Add(GameplayTags.CombatSocket_3, GameplayTags.Montage_Attack_3);
	GameplayTags.CombatSocketToMontage.Add(GameplayTags.CombatSocket_4, GameplayTags.Montage_Attack_4);

#pragma endregion

	
#pragma region InputTag
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"));

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"));

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"));

	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.5"));

	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.5"));

	GameplayTags.InputTag_A = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.A"));

	GameplayTags.InputTag_S = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.S"));

	GameplayTags.InputTag_S = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Shift"));

#pragma endregion

	
#pragma region BolckTag
	
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"));
	
	GameplayTags.Player_Block_Released = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.Released"));

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"));

	
#pragma endregion
}
