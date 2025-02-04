// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CavePlayerState.generated.h"

class ULevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedDelegate, int32 /*  New Stat */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedDelegate, const int32 /* New Level */, const bool /* Is Level Up */);

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACavePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACavePlayerState();

	/* Engine */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	/* end Engine */
	
	/* AbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* AbilitySystemInterface  */
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	UFUNCTION(Server, Reliable)
	void AddClearedDungeon(const FName& DungeonID);
	
	TArray<FName> GetClearedDungeonsList() const { return ClearedDungeons; };
	void SetClearedDungeonList(const TArray<FName>& InClearedDungeons);
	
	bool IsDungeonCleared(const FName& DungeonID) const;
	

#pragma region Character Attribute

	FOnLevelChangedDelegate OnLevelChangeDelegate;
	FOnPlayerStatChangedDelegate OnXPChangeDelegate;
	FOnPlayerStatChangedDelegate OnAttributePointsChangedDelegate;
	FOnPlayerStatChangedDelegate OnSpellPointsChangedDelegate;

	int32 GetPlayerLevel() const { return Level; }
	int32 GetXP() const { return XP; }
	int32 GetAttributePoints() const { return AttributePoints; }
	int32 GetSpellPoints() const { return SpellPoints; }

	void SetPlayerLevel(const int32 NewLevel);
	void SetXP(const int32 NewXP);
	void SetAttributePoints(const int32 NewAttributePoints);
	void SetSpellPoints(const int32 NewSpellPoints);

	void AddToLevel(const int32 InLevel);
	void AddTpXP(const int32 InXP);
	void AddToAttributePoints(const int32 InAttributePoints);
	void AddToSpellPoints(const int32 InSpellPoints);

#pragma endregion

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(Replicated)
	TArray<FName> ClearedDungeons;


#pragma region Character Attribute
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldPoint);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldPoint);

#pragma endregion
	
	
};
