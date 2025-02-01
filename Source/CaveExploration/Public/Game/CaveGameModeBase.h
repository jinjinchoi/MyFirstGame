// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CaveGameModeBase.generated.h"

class UCaveSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfoDataAsset;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACaveGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfoDataAsset> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	/* Save Logic */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> CaveSaveGameClass;

	void SaveSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex) const;
	UCaveSaveGame* GetSaveSlotData(const FString& SlotName, const int32 SlotIndex) const;
	static void DeleteSlot(const FString& SlotName, const int32 SlotIndex);
	void TravelMap(UMVVM_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void BeginPlay() override;
	
};
