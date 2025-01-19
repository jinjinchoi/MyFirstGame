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

	const FGameplayEffectContextHandle& EffectContext = Spec.GetContext();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTag;
	EvaluateParams.TargetTags = TargetTag;
	
	const FGameplayTag& DamageType = UCaveFunctionLibrary::GetDamageType(EffectContext);

	float Damage = Spec.GetSetByCallerMagnitude(DamageType, false);
	if (Damage > 0.f)
	{
		const FGameplayEffectAttributeCaptureDefinition& ResistanceDef = TagsToCapturedAttributes[DamageType];
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceDef, EvaluateParams, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 1.f);

		// TODO : 방사형 데미지 적용하려면 여기서 해야함
		
		Damage *= (100.f - Resistance) / 100.f;
		
	}

	
	

	
}
