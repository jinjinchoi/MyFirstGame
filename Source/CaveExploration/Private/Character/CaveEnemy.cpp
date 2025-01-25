// Copyright Jonwoo-Choi


#include "Character//CaveEnemy.h"

#include "CaveFunctionLibrary.h"
#include "CaveGameplayTags.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "AbilitySystem/CaveAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/CaveUserWidget.h"

ACaveEnemy::ACaveEnemy()
{
	Tags.Add(FName("Enemy"));
	
	AbilitySystemComponent = CreateDefaultSubobject<UCaveAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCaveAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	BaseWalkSpeed = 250.f;
}

int32 ACaveEnemy::GetCharacterLevel_Implementation() const
{
	return EnemyLevel;
}

void ACaveEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		UCaveFunctionLibrary::GiveStartupAbilities(this , AbilitySystemComponent);
	}

	if (UCaveUserWidget* CaveUserWidget = Cast<UCaveUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		CaveUserWidget->SetWidgetController(this);
	}

	if (const UCaveAttributeSet* CaveAS = Cast<UCaveAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CaveAS->GetHealthAttribute()).AddLambda
		([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChangedDelegate.Broadcast(Data.NewValue);
		});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CaveAS->GetMaxHealthAttribute()).AddLambda
		([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
		});
		
		OnHealthChangedDelegate.Broadcast(CaveAS->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(CaveAS->GetMaxHealth());
	}

	ReactGameplayTagChanged();

}

void ACaveEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	
}

void ACaveEnemy::InitializeDefaultAttributes() const
{
	UCaveFunctionLibrary::InitializeDefaultAttribute(this, CharacterClass, EnemyLevel, AbilitySystemComponent);
}

void ACaveEnemy::ReactGameplayTagChanged()
{
	Super::ReactGameplayTagChanged();
	
	const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();
	
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Abilities_Common_HitReact, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ACaveEnemy::HitReactTagChange);

	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Abilities_Common_Death, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ACaveEnemy::DeathReactTagChange);

	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Debuff_Type_Frozen, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ACaveEnemy::FrozenTagChanged);

	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Debuff_Type_Stun, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ACaveEnemy::StunTagChanged);
}

void ACaveEnemy::HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void ACaveEnemy::DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::DeathReactTagChange(CallbackTag, NewCount);
	SetLifeSpan(LifeSpan);
	
	AutoPossessAI = EAutoPossessAI::Disabled;
	
	if (AController* AIController = GetController())
	{
		AIController->UnPossess();
	}
	
}

void ACaveEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}
