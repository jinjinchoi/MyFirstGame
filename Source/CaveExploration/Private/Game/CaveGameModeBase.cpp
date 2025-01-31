// Copyright Jonwoo-Choi


#include "Game/CaveGameModeBase.h"

#include "Game/CaveSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void ACaveGameModeBase::SaveSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex) const
{
	check(CaveSaveGameClass);
	
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(CaveSaveGameClass);
	UCaveSaveGame* CaveSaveGame = Cast<UCaveSaveGame>(SaveGameObject);
	
	CaveSaveGame->SavedDate = LoadSlot->GetSavedDate();
	CaveSaveGame->SlotStatus = Taken;

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
