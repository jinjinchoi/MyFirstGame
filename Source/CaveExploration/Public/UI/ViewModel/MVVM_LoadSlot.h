// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/CaveSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndexDelegate, int32, WidgetSwitcherIndex);
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeSlot() const;

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndexDelegate SetWidgetSwitcherIndexDelegate;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	void SetMapAssetName(const FString& InMapAssetName);
	FString GetMapAssetName() const { return MapAssetName ;}
	
	/* Field Notifies */
	void SetLoadSlotName(const FString& InLoadSlotName);
	void SetLoadSlotIndex(const int32 InLoadSlotIndex);
	void SetSavedDate(const FString& InSavedDate);
	void SetbTakenSlotClickEnabled(const bool InbEnabled);
	void SetMapName(const FString& InMapName);
	void SetPlayerLevel(const int32 InPlayerLevel);

	FString GetLoadSlotName() const { return LoadSlotName; }
	int32 GetLoadSlotIndex() const { return  LoadSlotIndex; }
	FString GetSavedDate() const { return SavedDate; }
	bool GetbTakenSlotClickEnabled() const { return bTakenSlotClickEnabled; }
	FString GetMapName() const { return MapName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	FString LoadSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	int32 LoadSlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	FString SavedDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	bool bTakenSlotClickEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	int32 PlayerLevel;

	UPROPERTY(Setter, Getter)
	FString MapAssetName;
};
