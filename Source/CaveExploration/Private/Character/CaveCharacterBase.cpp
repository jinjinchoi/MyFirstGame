// Copyright Jonwoo-Choi


#include "Character//CaveCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "CaveGameplayTags.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "AbilitySystem/Niagara/DebuffNiagaraComponent.h"
#include "CaveExploration/CaveExploration.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


ACaveCharacterBase::ACaveCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FCaveGameplayTags::Get().Debuff_Type_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FCaveGameplayTags::Get().Debuff_Type_Stun;

	FrozenDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("FrozenDebuffComponent");
	FrozenDebuffComponent->SetupAttachment(GetRootComponent());
	FrozenDebuffComponent->DebuffTag = FCaveGameplayTags::Get().Debuff_Type_Frozen;
	
}


void ACaveCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACaveCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACaveCharacterBase, bIsStunned);
	DOREPLIFETIME(ACaveCharacterBase, bIsBurned);
	DOREPLIFETIME(ACaveCharacterBase, bIsFrozen);
}

UAbilitySystemComponent* ACaveCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


FVector ACaveCharacterBase::GetWeaponSocketLocation_Implementation()
{
	if (IsValid(Weapon) && Weapon->GetStaticMesh())
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}

	return GetMesh()->GetSocketLocation(WeaponTipSocketName);
}

UAnimMontage* ACaveCharacterBase::GetHitReactMontage_Implementation()
{
	if (HitReactMontages.Num() == 0) return nullptr;

	return HitReactMontages[FMath::RandRange(0, HitReactMontages.Num() - 1)];
	
}

UAnimMontage* ACaveCharacterBase::GetDeathMontage_Implementation()
{
	if (DeathMontages.Num() == 0) return nullptr;

	return DeathMontages[FMath::RandRange(0, DeathMontages.Num() - 1)];
}

bool ACaveCharacterBase::IsDead_Implementation() const
{
	return bIsDead;
}

ECharacterClass ACaveCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

USoundBase* ACaveCharacterBase::GetHitImpactSound_Implementation()
{
	return HitImpactSound;
}

FOnASCRegistered ACaveCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeath& ACaveCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}


void ACaveCharacterBase::InitAbilityActorInfo()
{
}

void ACaveCharacterBase::ReactGameplayTagChanged()
{
	check(AbilitySystemComponent);
}

void ACaveCharacterBase::HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void ACaveCharacterBase::DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsDead = NewCount > 0;
	if (!bIsDead) return;

	OnDeath.Broadcast(this);
	
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Weapon->SetCollisionObjectType(ECC_PhysicsBody);
	Weapon->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Weapon->SetSimulatePhysics(true);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
}



void ACaveCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(PrimaryAttributesClass, 1.f);
	ApplyEffectToSelf(SecondaryAttributesClass, 1.f);
	ApplyEffectToSelf(VitalAttributesClass, 1.f);
	ApplyEffectToSelf(ResistanceClass, 1.f);
}

void ACaveCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const
{
	check(EffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACaveCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority()) return;

	UCaveAbilitySystemComponent* CaveASC = Cast<UCaveAbilitySystemComponent>(AbilitySystemComponent);
	CaveASC->AddCharacterAbilities(StartupAbilities);
	CaveASC->AddCharacterStartupPassiveAbilities(StartupPassiveAbilities);
	CaveASC->AddCharacterStartupInteractionAbilities(StartupInteractionAbilities);
	
}

void ACaveCharacterBase::FrozenTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bFrozen = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bFrozen ? BaseWalkSpeed / 3 : BaseWalkSpeed;
	bIsFrozen = bFrozen;
}

void ACaveCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bStun = NewCount > 0;
	bIsStunned = bStun;
}

void ACaveCharacterBase::BurnedTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bBurned = NewCount > 0;
	bIsBurned = bBurned;
}


void ACaveCharacterBase::OnRep_Stunned()
{
}

void ACaveCharacterBase::OnRep_Burned()
{
}

void ACaveCharacterBase::OnRep_Frozen()
{
}
