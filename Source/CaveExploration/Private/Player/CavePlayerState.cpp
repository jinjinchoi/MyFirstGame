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

void ACavePlayerState::OnRep_Level(int32 OldLevel)
{

}

void ACavePlayerState::OnRep_XP(int32 OldXP)
{
}

void ACavePlayerState::OnRep_AttributePoints(int32 OldPoint)
{
}

void ACavePlayerState::OnRep_SpellPoints(int32 OldPoint)
{
}
