// Copyright Jonwoo-Choi


#include "Game/CaveGameModeBase.h"

#include "Game/CaveGameInstance.h"
#include "Game/CaveSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "GameFramework/Character.h"


void ACaveGameModeBase::SaveSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex) const
{
	check(CaveSaveGameClass);

	DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(CaveSaveGameClass);
	UCaveSaveGame* CaveSaveGame = Cast<UCaveSaveGame>(SaveGameObject);
	
	CaveSaveGame->SavedDate = LoadSlot->GetSavedDate();
	CaveSaveGame->SlotStatus = Taken;
	CaveSaveGame->MapName = LoadSlot->GetMapName();
	CaveSaveGame->MapAssetName = DefaultMap.ToSoftObjectPath().GetAssetName();
	CaveSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(CaveSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

UCaveSaveGame* ACaveGameModeBase::GetSaveSlotData(const FString& SlotName, const int32 SlotIndex) const
{
	check(CaveSaveGameClass);
	
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName,SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(CaveSaveGameClass);
	}
	return Cast<UCaveSaveGame>(SaveGameObject);
}

void ACaveGameModeBase::DeleteSlot(const FString& SlotName, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void ACaveGameModeBase::TravelMap(UMVVM_LoadSlot* Slot)
{
	for (TSoftObjectPtr<UWorld> Map : Maps)
	{
		if (Map.ToSoftObjectPath().GetAssetName() == Slot->GetMapAssetName())
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Map);
		}
	}
}

UCaveSaveGame* ACaveGameModeBase::RetrieveSaveGameData() const
{
	const UCaveGameInstance* CaveGameInstance = Cast<UCaveGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = CaveGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = CaveGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void ACaveGameModeBase::SaveInGameProgressData(UCaveSaveGame* SaveObject)
{
	UCaveGameInstance* CaveGameInstance = Cast<UCaveGameInstance>(GetGameInstance());
	const FString InGameLoadSlotName = CaveGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = CaveGameInstance->LoadSlotIndex;
	CaveGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	const FDateTime Now = FDateTime::Now();
	const FString CurrentDate = FString::Printf(
		TEXT("%04d/%02d/%02d  %02d:%02d:%02d"), Now.GetYear(), Now.GetMonth(),  Now.GetDay(),Now.GetHour(), Now.GetMinute(), Now.GetSecond());
	SaveObject->SavedDate = CurrentDate;
	
	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

AActor* ACaveGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UCaveGameInstance* CaveGameInstance = Cast<UCaveGameInstance>(GetGameInstance());
	TArray<AActor*> PlayerStartActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartActors);

	if (PlayerStartActors.Num() > 0)
	{
		AActor* SelectedPlayerStart = PlayerStartActors[0];
		for (AActor* Actor : PlayerStartActors)
		{
			if (const APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == CaveGameInstance->PlayerStartTag)
				{
					SelectedPlayerStart = Actor;
					break;
				}
			}
		}

		return SelectedPlayerStart;
	}

	return nullptr;
}

void ACaveGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	UCaveSaveGame* SaveGame = RetrieveSaveGameData();
	if (!IsValid(SaveGame)) return;

	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

void ACaveGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

