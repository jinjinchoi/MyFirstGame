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
#include "AbilitySystem/Niagara/DebuffNiagaraComponent.h"
#include "Game/CaveGameInstance.h"
#include "Game/CaveGameModeBase.h"
#include "Game/CaveSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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
			LoadProgress();
		}
		else
		{
			// 서버가 없기 때문에 임시로
			AddCharacterAbilities();
		}
	}
	

}

void ACavePlayerCharacter::LoadProgress()
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
				CavePlayerState->SetClearedDungeonList(SaveData->ClearedDungeons);
			}
			
			UCaveFunctionLibrary::InitializeDefaultAttributeFromSaveData(this, AbilitySystemComponent, SaveData);
			ApplyEffectToSelf(SecondaryAttributesClass, 1.f);
			ApplyEffectToSelf(VitalAttributesClass, 1.f);
			ApplyEffectToSelf(ResistanceClass, 1.f);
		}
		
	}
}

void ACavePlayerCharacter::SaveProgress_Implementation(const FName& CheckPointTag, const FString& CheckPointName)
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
			SaveData->ClearedDungeons = CavePlayerState->GetClearedDungeonsList();
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
			FString MapAssetName = World->GetMapName();
			MapAssetName.RemoveFromStart(World->StreamingLevelsPrefix);
			SaveData->MapAssetName = MapAssetName;
		}

		
		SaveData->bFirstTimeLoadIn = false;
		SaveData->MapName = CheckPointName;
		CaveGameMode->SaveInGameProgressData(SaveData);
		
	}
}

void ACavePlayerCharacter::AddClearedDungeon_Implementation(const FName& DungeonID)
{
	ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);
	CavePlayerState->AddClearedDungeon(DungeonID);
}

bool ACavePlayerCharacter::IsDungeonCleared_Implementation(const FName& DungeonID) const
{
	const ACavePlayerState* CavePlayerState = GetPlayerState<ACavePlayerState>();
	check(CavePlayerState);

	return CavePlayerState->IsDungeonCleared(DungeonID);
}

FVector ACavePlayerCharacter::GetCharacterMoveDirection_Implementation() const
{
	return MoveDirection;
}

void ACavePlayerCharacter::SetCharacterMoveDirection_Implementation(const FVector& NewDirection)
{
	ServerSetMoveDirection(NewDirection);
}

void ACavePlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACavePlayerCharacter, MoveDirection);
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

	/* 이 아래 부분은 멀티 플레이를 위한 부분으로 서버가 존재할 경우 다른 방법으로 구현해야 함 */
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

	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Debuff_Type_Burn)
		.AddUObject(this, &ACavePlayerCharacter::BurnedTagChanged);
	
}

void ACavePlayerCharacter::HitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
}

void ACavePlayerCharacter::DeathReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::DeathReactTagChange(CallbackTag, NewCount);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
	{
		if (ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			UCaveGameInstance* CaveGameInstance = Cast<UCaveGameInstance>(CaveGameMode->GetGameInstance());
			if (CaveGameInstance->bIsSinglePlay)
			{
				CaveGameMode->PlayerDied(this);
				return;
			}
		}
		
		UGameplayStatics::OpenLevel(this, RespawnWorldNameForMultiPlay);
	});

	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);

	
}

void ACavePlayerCharacter::OnRep_Stunned()
{
	if (bIsStunned)
	{
		StunDebuffComponent->Activate();
	}
	else
	{
		StunDebuffComponent->Deactivate();
	}
}

void ACavePlayerCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void ACavePlayerCharacter::OnRep_Frozen()
{
	if (bIsFrozen)
	{
		FrozenDebuffComponent->Activate();
	}
	else
	{
		FrozenDebuffComponent->Deactivate();
	}
}

UCaveAttributeSet* ACavePlayerCharacter::GetCaveAttributeSet() const
{
	return Cast<UCaveAttributeSet>(AttributeSet);
}

void ACavePlayerCharacter::ServerSetMoveDirection_Implementation(const FVector& NewMoveDirection)
{
	MoveDirection = NewMoveDirection;
}
