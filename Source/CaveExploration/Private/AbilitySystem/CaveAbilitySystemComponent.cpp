// Copyright Jonwoo-Choi


#include "AbilitySystem/CaveAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CaveFunctionLibrary.h"
#include "CaveGameplayTags.h"
#include "AbilitySystem/Abilities/CaveGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Interaction/PlayerInterface.h"

void UCaveAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UCaveGameplayAbility* CaveAbility = Cast<UCaveGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(CaveAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FCaveGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UCaveAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UCaveAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UCaveAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UCaveAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UCaveAbilitySystemComponent::ForEachAbility(const FForEachAbilityDelegate& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

void UCaveAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UCaveAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

FGameplayTag UCaveAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UCaveAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UCaveAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}
	
	return FGameplayTag();
}

int32 UCaveAbilitySystemComponent::GetAbilityLevelFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return Spec->Level;
	}

	return 0;
}


FGameplayAbilitySpec* UCaveAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability->AbilityTags)
		{
			if (Tag.MatchesTagExact(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayTag UCaveAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}

	return FGameplayTag();
}



FGameplayTag UCaveAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}

	return FGameplayTag();
}

bool UCaveAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	return true;
}

FGameplayAbilitySpec* UCaveAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

void UCaveAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	// 어빌리티에 있는 InputTag를 지우고,
	ClearSlot(&Spec);
	// 새로운 InputTag 추가.
	Spec.DynamicAbilityTags.AddTag(Slot);
}


void UCaveAbilitySystemComponent::ServerSpellSpendPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}

		const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();
		FGameplayTag StatusTag = GetStatusFromSpec(*AbilitySpec);

		if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);
			StatusTag = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityStatus(AbilityTag, StatusTag, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}


void UCaveAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		FCaveGameplayTags GameplayTags = FCaveGameplayTags::Get();

		// 이미 장착 중인 어빌리티인지 확인하기 위하여 Input Tag를 가져옴
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& StatusTag = GetStatusFromSpec(*AbilitySpec);

		// Status 유효성 확인. Unlocked(스킬 포인트를 썼지만 장착은 안함) 또는 Equipped 상태에서만 장착 과정을 진행해햐함
		const bool bStatusValid = StatusTag == GameplayTags.Abilities_Status_Equipped || StatusTag == GameplayTags.Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			// 슬롯이 비어있나 확인하고 비어있지 않으면
			if (!SlotIsEmpty(Slot))
			{
				// Slot에 이미 할당 되어있는 어빌리티 불러오기
				if (FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(Slot))
				{
					// 장착하려는 슬롯의 어빌리티와 스롯에 이미 할당 되어 있는 어빌리티가 동일하면 Early Return
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
						return;
					}
					// 다를 경우 해당 슬롯에 있는 어빌리티의 InputTag를 삭제
					ClearSlot(SpecWithSlot);
					// Status 태그도 교체.
					SpecWithSlot->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Equipped);
					SpecWithSlot->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);
				}
			}

			// 어빌리티에 새로운 InputTag를 할당
			AssignSlotToAbility(*AbilitySpec, Slot);
			// 장착 중인 어빌리티가 아니라면 Status 변경
			if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
			ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
		}
	}
}


void UCaveAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, Slot, PrevSlot);
}

void UCaveAbilitySystemComponent::ClearAbilityOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}
bool UCaveAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.DynamicAbilityTags.HasTagExact(Slot);
}

void UCaveAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag& Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
}


void UCaveAbilitySystemComponent::UpdateAbilityStatuses(const int32 Level)
{
	if (!GetAvatarActor()->HasAuthority()) return;

	UAbilityInfo* AbilityInfo = UCaveFunctionLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FCaveAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid() || Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FCaveGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FCaveGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

void UCaveAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}


void UCaveAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}
