// Copyright Jonwoo-Choi


#include "Character//CaveCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "CaveExploration/CaveExploration.h"


ACaveCharacterBase::ACaveCharacterBase()
{

	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

UAbilitySystemComponent* ACaveCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector ACaveCharacterBase::GetWeaponSocketLocation_Implementation()
{
	if (IsValid(Weapon))
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


void ACaveCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void ACaveCharacterBase::InitAbilityActorInfo()
{
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

void ACaveCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	UCaveAbilitySystemComponent* CaveASC = Cast<UCaveAbilitySystemComponent>(AbilitySystemComponent);
	CaveASC->AddCharacterAbilities(StartupAbilities);
	
	
}
