// Copyright Jonwoo-Choi


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot() const
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetIntValue();
	SetWidgetSwitcherIndexDelegate.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetLoadSlotName(const FString& InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UMVVM_LoadSlot::SetLoadSlotIndex(const int32 InLoadSlotIndex)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotIndex, InLoadSlotIndex);
}

void UMVVM_LoadSlot::SetSavedDate(const FString& InSavedDate)
{
	UE_MVVM_SET_PROPERTY_VALUE(SavedDate, InSavedDate);
}
