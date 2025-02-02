// Copyright Jonwoo-Choi


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/CaveGameInstance.h"
#include "Game/CaveGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlot()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotIndex(0);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlots.Add(0, LoadSlot_0);

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotIndex(1);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotIndex(2);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(const int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::CreateSlotButtonPressed(const int32 Slot)
{
	const ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetWidgetSwitcherIndexDelegate.Broadcast(1);

	// 날짜 저장
	const FDateTime Now = FDateTime::Now();
	const FString CurrentDate = FString::Printf(
		TEXT("%04d/%02d/%02d  %02d:%02d:%02d"), Now.GetYear(), Now.GetMonth(),  Now.GetDay(),Now.GetHour(), Now.GetMinute(), Now.GetSecond());
	
	LoadSlots[Slot]->SetSavedDate(CurrentDate);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->SetMapName(CaveGameMode->DefaultMapName);
	LoadSlots[Slot]->PlayerStartTag = CaveGameMode->DefaultPlayerStartTag;
	LoadSlots[Slot]->SetPlayerLevel(1);

	CaveGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();

	UCaveGameInstance* CaveGameInstance = Cast<UCaveGameInstance>(CaveGameMode->GetGameInstance());
	CaveGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	CaveGameInstance->LoadSlotIndex = LoadSlots[Slot]->GetLoadSlotIndex();
	CaveGameInstance->PlayerStartTag = CaveGameMode->DefaultPlayerStartTag;
	
}

void UMVVM_LoadScreen::TakenSlotPressed(int32 Slot)
{
	for (const TTuple<int, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
	{
		LoadSlot.Value->SetbTakenSlotClickEnabled(LoadSlot.Key != Slot);
	}

	SetbPlayButtonEnabled(true);
	SetbDeleteButtonEnabled(true);
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteSavedSlot() const
{
	if (IsValid(SelectedSlot))
	{
		ACaveGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->GetLoadSlotIndex());
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->SetbTakenSlotClickEnabled(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this));
	UCaveGameInstance* CaveGameInstance = Cast<UCaveGameInstance>(CaveGameMode->GetGameInstance());
	
	if (IsValid(SelectedSlot))
	{
		CaveGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
		CaveGameInstance->bIsSinglePlay = true;
		CaveGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
		CaveGameInstance->LoadSlotIndex = SelectedSlot->GetLoadSlotIndex();
		
		CaveGameMode->TravelMap(SelectedSlot);
	}
}

void UMVVM_LoadScreen::LoadData()
{
	ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TTuple<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
	{
		const UCaveSaveGame* SaveObject = CaveGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
		LoadSlot.Value->SlotStatus = SaveObject->SlotStatus;
		LoadSlot.Value->SetSavedDate(SaveObject->SavedDate);
		LoadSlot.Value->InitializeSlot();
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
	}
}

void UMVVM_LoadScreen::SetbPlayButtonEnabled(const bool IsEnabled)
{
	UE_MVVM_SET_PROPERTY_VALUE(bPlayButtonEnabled, IsEnabled);
}

void UMVVM_LoadScreen::SetbDeleteButtonEnabled(const bool IsEnabled)
{
	UE_MVVM_SET_PROPERTY_VALUE(bDeleteButtonEnabled, IsEnabled);
}
