// Copyright Jonwoo-Choi


#include "Game/CaveGameModeBase.h"

#include "Game/CaveSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"


void ACaveGameModeBase::SaveSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex) const
{
	check(CaveSaveGameClass);

	DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(CaveSaveGameClass);
	UCaveSaveGame* CaveSaveGame = Cast<UCaveSaveGame>(SaveGameObject);
	
	CaveSaveGame->SavedDate = LoadSlot->GetSavedDate();
	CaveSaveGame->SlotStatus = Taken;
	CaveSaveGame->MapName = LoadSlot->GetMapName();

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
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->GetLoadSlotIndex();
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

void ACaveGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);
}

