// Copyright Jonwoo-Choi


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "CaveFunctionLibrary.h"
#include "CaveGameplayTags.h"
#include "AbilitySystem/CaveAttributeSet.h"

struct CaveAttributesStatics
{
	/**
	*	P : Property Name - 속성을 캡쳐할 때 사용할 변수명 (DEFINE 매크로와 연결되어 있어 FGameplayAttributeData 변수명과 동일해야함)
	*/

	// Source Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	// Target Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	
	CaveAttributesStatics()
	{
		/**
		 *	S : Source Class - Attribute Set 클래스. 약칭이 S인 이유는 Attribute를 가져올 Source 라는 의미로 추정
		 *	P : Property Name - 캡처할 속성의 이름 
		 *	T : Target - 속성을 가져올 대상
		 *	B : Boolean - 스냅샷 여부
		 */
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, Intelligence, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, CriticalHitDamage, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCaveAttributeSet, PhysicalResistance, Target, false);
	}
};

static const CaveAttributesStatics& AttributesStatics()
{
	static CaveAttributesStatics AStatics;
	return AStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(AttributesStatics().StrengthDef);
	RelevantAttributesToCapture.Add(AttributesStatics().IntelligenceDef);
	RelevantAttributesToCapture.Add(AttributesStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(AttributesStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(AttributesStatics().CriticalHitDamageDef);
	
	RelevantAttributesToCapture.Add(AttributesStatics().ArmorDef);
	RelevantAttributesToCapture.Add(AttributesStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(AttributesStatics().IceResistanceDef);
	RelevantAttributesToCapture.Add(AttributesStatics().PhysicalResistanceDef);
	
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();
	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCapturedAttributes;
	TagsToCapturedAttributes.Add(GameplayTags.Damage_Fire, AttributesStatics().FireResistanceDef);
	TagsToCapturedAttributes.Add(GameplayTags.Damage_Ice, AttributesStatics().IceResistanceDef);
	TagsToCapturedAttributes.Add(GameplayTags.Damage_Physical, AttributesStatics().PhysicalResistanceDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FGameplayEffectContextHandle EffectContext = Spec.GetContext();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTag;
	EvaluateParams.TargetTags = TargetTag;
	
	const FGameplayTag& DamageType = UCaveFunctionLibrary::GetDamageType(EffectContext);

	// Set By Caller로 데미지 불러오기
	float Damage = Spec.GetSetByCallerMagnitude(DamageType, false);

	// 데미지 타입에 따른 추가 데미지 계산
	if (DamageType.MatchesTagExact(GameplayTags.Damage_Fire) || DamageType.MatchesTagExact(GameplayTags.Damage_Ice))
	{
		float Intelligence = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributesStatics().IntelligenceDef, EvaluateParams, Intelligence);
		Intelligence = FMath::Max(Intelligence, 0.f);
		
		Damage += (Intelligence * 0.2);
	}
	if (DamageType.MatchesTagExact(GameplayTags.Damage_Physical))
	{
		float Strength = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributesStatics().StrengthDef, EvaluateParams, Strength);
		Strength = FMath::Max(Strength, 0.f);

		Damage += (Strength * 0.2);
	}
	
	// 데미지 저항 계산
	if (Damage > 0.f)
	{
		const FGameplayEffectAttributeCaptureDefinition& ResistanceDef = TagsToCapturedAttributes[DamageType];
		float TargetResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceDef, EvaluateParams, TargetResistance);
		TargetResistance = FMath::Clamp(TargetResistance, 0.f, 100.f);

		// TODO : 방사형 데미지 적용하려면 여기서 해야함
		
		Damage *= (100.f - TargetResistance) / 100.f;
	}

	// 방어력 & 방어력 관통력 계산
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributesStatics().ArmorDef, EvaluateParams, TargetArmor);
	
	float ArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributesStatics().ArmorPenetrationDef, EvaluateParams, ArmorPenetration);
	ArmorPenetration = FMath::Clamp(ArmorPenetration, 0.f, 100.f);
	
	float EffectiveTargetArmor = TargetArmor * (100.f - ArmorPenetration) / 100.f;

	Damage *= (100 - EffectiveTargetArmor * 0.25f) / 100.f;

	// 치명타 공격 계산
	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributesStatics().CriticalHitChanceDef, EvaluateParams, CriticalHitChance);
	CriticalHitChance = FMath::Clamp(CriticalHitChance, 0.f, 100.f);

	float CriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributesStatics().CriticalHitDamageDef, EvaluateParams, CriticalHitDamage);
	CriticalHitDamage = FMath::Max(CriticalHitDamage, 0.f);

	const bool bCriticalHit = FMath::FRandRange(1.f, 100.f) < CriticalHitChance;
	UCaveFunctionLibrary::SetIsCriticalHit(EffectContext, bCriticalHit);

	Damage = bCriticalHit ? 1.5 * Damage + CriticalHitDamage : Damage;

	
	const FGameplayModifierEvaluatedData EvaluatedData(UCaveAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, 0);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}

