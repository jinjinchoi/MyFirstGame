// Copyright Jonwoo-Choi


#include "Player/CavePlayerState.h"

#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "AbilitySystem/CaveAttributeSet.h"
#include "Net/UnrealNetwork.h"

ACavePlayerState::ACavePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCaveAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCaveAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void ACavePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACavePlayerState, Level);
	DOREPLIFETIME(ACavePlayerState, XP);
	DOREPLIFETIME(ACavePlayerState, AttributePoints);
	DOREPLIFETIME(ACavePlayerState, SpellPoints);
	
}

UAbilitySystemComponent* ACavePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACavePlayerState::SetPlayerLevel(const int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChangeDelegate.Broadcast(Level, false);
}

void ACavePlayerState::SetXP(const int32 NewXP)
{
	XP = NewXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void ACavePlayerState::SetAttributePoints(const int32 NewAttributePoints)
{
	AttributePoints = NewAttributePoints;
	OnSpellPointsChangedDelegate.Broadcast(AttributePoints);
}

void ACavePlayerState::SetSpellPoints(const int32 NewSpellPoints)
{
	SpellPoints = NewSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ACavePlayerState::AddToLevel(const int32 InLevel)
{
	Level += InLevel;
	OnLevelChangeDelegate.Broadcast(Level, true);
}

void ACavePlayerState::AddTpXP(const int32 InXP)
{
	XP += InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void ACavePlayerState::AddToAttributePoints(const int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ACavePlayerState::AddToSpellPoints(const int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ACavePlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangeDelegate.Broadcast(Level, true);
}

void ACavePlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangeDelegate.Broadcast(XP);
}

void ACavePlayerState::OnRep_AttributePoints(int32 OldPoint)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ACavePlayerState::OnRep_SpellPoints(int32 OldPoint)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
