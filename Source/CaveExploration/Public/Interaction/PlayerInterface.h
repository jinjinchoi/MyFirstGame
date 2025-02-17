// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAVEEXPLORATION_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 InLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 InLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InPlayerComboAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndPlayerComboAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetMagicCircleLocation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& CheckPointTag, const FString& CheckPointName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddClearedDungeon(const FName& DungeonID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDungeonCleared(const FName& DungeonID) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCharacterMoveDirection() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCharacterMoveDirection(const FVector& NewDirection);
	
};
