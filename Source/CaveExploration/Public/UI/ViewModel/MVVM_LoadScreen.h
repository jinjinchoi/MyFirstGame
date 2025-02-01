// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeLoadSlot();

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	/* 저장 데이터가 없는 슬롯을 누를 때 */
	UFUNCTION(BlueprintCallable)
	void CreateSlotButtonPressed(const int32 Slot);

	/* 저장된 슬롯을 누를 때 */
	UFUNCTION(BlueprintCallable)
	void TakenSlotPressed(const int32 Slot);

	/* 저장 슬롯 삭제 함수 */
	UFUNCTION(BlueprintCallable)
	void DeleteSavedSlot() const;

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();
	
	void LoadData();

	/* Field Notifies */
	void SetbPlayButtonEnabled(const bool IsEnabled);
	void SetbDeleteButtonEnabled(const bool IsEnabled);

	bool GetbPlayButtonEnabled() const { return bPlayButtonEnabled; } ;
	bool GetbDeleteButtonEnabled() const { return bDeleteButtonEnabled; };
	

private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot *> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot; 
	

	/* Field Notifies */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	bool bPlayButtonEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	bool bDeleteButtonEnabled = false;
};
