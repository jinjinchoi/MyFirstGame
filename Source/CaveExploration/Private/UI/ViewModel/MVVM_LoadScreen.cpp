// Copyright Jonwoo-Choi


#include "UI/ViewModel/MVVM_LoadScreen.h"

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
	LoadSlots[Slot]->SetWidgetSwitcherIndexDelegate.Broadcast(1);

	// 날짜 저장
	const FDateTime Now = FDateTime::Now();
	const FString CurrentDate = FString::Printf(
		TEXT("%04d/%02d/%02d  %02d:%02d:%02d"), Now.GetYear(), Now.GetMonth(),  Now.GetDay(),Now.GetHour(), Now.GetMinute(), Now.GetSecond());

	LoadSlots[Slot]->SetSavedDate(CurrentDate);
	LoadSlots[Slot]->SlotStatus = Taken;

	const ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this));
	CaveGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	
	LoadSlots[Slot]->InitializeSlot();
	
}

void UMVVM_LoadScreen::TakenSlotPressed(int32 Slot)
{
	
}

void UMVVM_LoadScreen::LoadData()
{
	ACaveGameModeBase* CaveGameMode = Cast<ACaveGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		const UCaveSaveGame* SaveObject = CaveGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
		LoadSlot.Value->SlotStatus = SaveObject->SlotStatus;
		LoadSlot.Value->SetSavedDate(SaveObject->SavedDate);
		LoadSlot.Value->InitializeSlot();
	}
}
