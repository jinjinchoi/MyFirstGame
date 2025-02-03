// Copyright Jonwoo-Choi


#include "Character//CavePlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "CaveFunctionLibrary.h"
#include "CaveGameplayTags.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "Player/CavePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CavePlayerController.h"
#include "UI/HUD/CaveHUD.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/CaveAbilitySystemComponent.h"
#include "AbilitySystem/CaveAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/CaveGameInstance.h"
#include "Game/CaveGameModeBase.h"
#include "Game/CaveSaveGame.h"
#include "Kismet/GameplayStatics.h"

ACavePlayerCharacter::ACavePlayerCharacter()
{
	Tags.Add(FName("Player"));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 750.f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = BaseRotationRate;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseMaxWalkSpeed;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	CharacterClass = ECharacterClass::Player;
	BaseWalkSpeed = 600.f;
}

void ACavePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	/* 멀티 플레이와 싱글플레이 구분하기 위한 부분으로 서버가 존재할 경우 변경해야함 */
	if (HasAuthority())
	{
		ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this));
		UCaveGameInstance* CaveGameInstance = Cast<UCaveGameInstance>(CaveGameMode->GetGameInstance());
		if (CaveGameInstance->bIsSinglePlay)
		{
			LoadProgrss();
		}
		else
		{
			// 서버가 없기 때문에 임시로
			AddCharacterAbilities();
		}
	}
	

}

void ACavePlayerCharacter::LoadProgrss()
{
	if (ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		UCaveSaveGame* SaveData = CaveGameMode->RetrieveSaveGameData();
		if (SaveData == nullptr) return;
		
		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			if (UCaveAbilitySystemComponent* CaveASC = Cast<UCaveAbilitySystemComponent>(AbilitySystemComponent))
			{
				CaveASC->AddCharacterAbilitiesFromSaveData(SaveData);
				CaveASC->AddCharacterStartupPassiveAbilities(StartupPassiveAbilities);
				CaveASC->AddCharacterStartupInteractionAbilities(StartupInteractionAbilities);
				CaveASC->UpdateAbilityStatuses(SaveData->PlayerLevel);
			}
			
			if (ACavePlayerState* CavePlayerState = Cast<ACavePlayerState>(GetPlayerState()))
			{
				CavePlayerState->SetPlayerLevel(SaveData->PlayerLevel);
				CavePlayerState->SetXP(SaveData->XP);
				CavePlayerState->SetAttributePoints(SaveData->AttributePoints);
				CavePlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			
			UCaveFunctionLibrary::InitializeDefaultAttributeFromSaveData(this, AbilitySystemComponent, SaveData);
			ApplyEffectToSelf(SecondaryAttributesClass, 1.f);
			ApplyEffectToSelf(VitalAttributesClass, 1.f);
			ApplyEffectToSelf(ResistanceClass, 1.f);
		}
		
	}
}

void ACavePlayerCharacter::SaveProgress_Implementation(const FName& CheckPointTag)
{
	if (!HasAuthority()) return;
	
	if (ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		UCaveSaveGame* SaveData = CaveGameMode->RetrieveSaveGameData();
		if (SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckPointTag;

		if (ACavePlayerState* CavePlayerState = Cast<ACavePlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = CavePlayerState->GetPlayerLevel();
			SaveData->XP = CavePlayerState->GetXP();
			SaveData->SpellPoints = CavePlayerState->GetSpellPoints();
			SaveData->AttributePoints = CavePlayerState->GetAttributePoints();
		}

		if (UCaveAttributeSet* CaveAttributeSet = GetCaveAttributeSet())
		{
			SaveData->Strength = CaveAttributeSet->GetStrength();
			SaveData->Intelligence = CaveAttributeSet->GetIntelligence();
			SaveData->Dexterity = CaveAttributeSet->GetDexterity();
			SaveData->Vigor = CaveAttributeSet->GetVigor();
		}

		SaveData->SaveAbilities.Empty();
		FForEachAbilityDelegate SaveAbilityDelegate;
		UCaveAbilitySystemComponent* CaveASC = Cast<UCaveAbilitySystemComponent>(AbilitySystemComponent);
		SaveAbilityDelegate.BindLambda([this, CaveASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(FCaveGameplayTags::Get().Abilities_Startup_Interaction)) return;
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(FCaveGameplayTags::Get().Abilities_Startup_Passive)) return;
			
			const FGameplayTag AbilityTag = CaveASC->GetAbilityTagFromSpec(AbilitySpec);
			UAbilityInfo* AbilityInfo = UCaveFunctionLibrary::GetAbilityInfo(this);
			FCaveAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			
			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbility = Info.AbilityClass;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilityInputTag = CaveASC->GetInputTagFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = CaveASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = FCaveGameplayTags::Get().Abilities_Type_Offensive;

			SaveData->SaveAbilities.AddUnique(SavedAbility);
			
			
		});
		CaveASC->ForEachAbility(SaveAbilityDelegate);

		if (const UWorld* World = GetWorld())
		{
			FString MapName = World->GetMapName();
			MapName.RemoveFromStart(World->StreamingLevelsPrefix);
			SaveData->MapPath = MapName;
		}

		
		SaveData->bFirstTimeLoadIn = false;
		CaveGameMode->SaveInGameProgressData(SaveData);
		
	}
}

void ACavePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

int32 ACavePlayerCharacter::GetCharacterLevel_Implementation() const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->GetPlayerLevel();
}

int32 ACavePlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ACavePlayerCharacter::GetXP_Implementation() const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->GetXP();
}

int32 ACavePlayerCharacter::GetAttributePoints_Implementation() const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->GetAttributePoints();
}

int32 ACavePlayerCharacter::GetSpellPoints_Implementation() const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->GetSpellPoints();
}

int32 ACavePlayerCharacter::GetAttributePointsReward_Implementation(int32 InLevel) const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->LevelUpInfo->LevelUpInformation[InLevel].AttributePointAward;
}

int32 ACavePlayerCharacter::GetSpellPointsReward_Implementation(int32 InLevel) const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	return CavePlayerState->LevelUpInfo->LevelUpInformation[InLevel].SpellPointAward;
}

void ACavePlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddTpXP(InXP);
}

void ACavePlayerCharacter::AddToPlayerLevel_Implementation(int32 InLevel)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddToLevel(InLevel);

	if (UCaveAbilitySystemComponent* CaveASC = Cast<UCaveAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		CaveASC->UpdateAbilityStatuses(CavePlayerState->GetPlayerLevel());
	}
}

void ACavePlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddToSpellPoints(InSpellPoints);
}

void ACavePlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddToAttributePoints(InAttributePoints);
}

void ACavePlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void ACavePlayerCharacter::InPlayerComboAttack_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 90.f, 0.f);
}

void ACavePlayerCharacter::EndPlayerComboAttack_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseMaxWalkSpeed;
	GetCharacterMovement()->RotationRate = BaseRotationRate;
}

void ACavePlayerCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (ACavePlayerController* CavePlayerController =  Cast<ACavePlayerController>(GetController()))
	{
		CavePlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void ACavePlayerCharacter::HideMagicCircle_Implementation()
{
	if (ACavePlayerController* CavePlayerController =  Cast<ACavePlayerController>(GetController()))
	{
		CavePlayerController->HideMagicCircle();
	}
	
}

FVector ACavePlayerCharacter::GetMagicCircleLocation_Implementation()
{
	if (ACavePlayerController* CavePlayerController =  Cast<ACavePlayerController>(GetController()))
	{
		return CavePlayerController->GetValidMagicCircleLocation();
	}

	return FVector::ZeroVector;

}


void ACavePlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}

void ACavePlayerCharacter::InitAbilityActorInfo()
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	if (!CavePlayerState) return;
	
	CavePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CavePlayerState, this);

	AbilitySystemComponent = CavePlayerState->GetAbilitySystemComponent();
	AttributeSet = CavePlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);

	if (ACavePlayerController* CavePlayerController =  Cast<ACavePlayerController>(GetController()))
	{
		if (ACaveHUD* CaveHUD = Cast<ACaveHUD>(CavePlayerController->GetHUD()))
		{
			CaveHUD->InitOverlay(CavePlayerController, CavePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	ReactGameplayTagChanged();

	/* 멀티 플레이와 싱글플레이 구분하기 위한 부분으로 서버가 존재할 경우 변경해야함 */
	if (!HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	else
	{
		ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this));
		UCaveGameInstance* CaveGameInstance = Cast<UCaveGameInstance>(CaveGameMode->GetGameInstance());
		if (!CaveGameInstance->bIsSinglePlay)
		{
			InitializeDefaultAttributes();
		}
	}
	
	
}

void ACavePlayerCharacter::ReactGameplayTagChanged()
{
	Super::ReactGameplayTagChanged();

	const FCaveGameplayTags& GameplayTags = FCaveGameplayTags::Get();
	
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Abilities_Common_HitReact)
		.AddUObject(this, &ACavePlayerCharacter::HitReactTagChange);

	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Abilities_Common_Death)
		.AddUObject(this, &ACavePlayerCharacter::DeathReactTagChange);

	AbilitySystemComponent->RegisterGameplayTagEvent(FCaveGameplayTags::Get().Debuff_Type_Frozen)
		.AddUObject(this, &ACavePlayerCharacter::FrozenTagChanged);

	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Debuff_Type_Stun)
		.AddUObject(this, &ACavePlayerCharacter::StunTagChanged);
}

void ACavePlayerCharacter::HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
}

void ACavePlayerCharacter::DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::DeathReactTagChange(CallbackTag, NewCount);
	
}

UCaveAttributeSet* ACavePlayerCharacter::GetCaveAttributeSet() const
{
	return Cast<UCaveAttributeSet>(AttributeSet);
}
