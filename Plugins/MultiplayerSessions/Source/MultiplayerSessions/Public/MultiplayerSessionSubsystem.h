// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionSubsystem.generated.h"


/**
 * Declaring our own custom delegate for the Menu class to bind callbacks to
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& /* SessionResults */, bool /* bWasSuccessful */);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionsComplete, EOnJoinSessionCompleteResult::Type /* Result*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * To handle session functionality. The Menu class will Call these
	 */
	void CreateSession(const int32 NumPublicConnections, const FString& MatchType);
	void FindSession(int32 MaxSearchResults);
	void JoinSession(FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

	/**
	 * Our own custom Delegates for the Menu class to bind callbacks to 
	 */
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionCompleteDelegate;
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsCompleteDelegate;
	FMultiplayerOnJoinSessionsComplete MultiplayerOnJoinSessionsCompleteDelegate;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionCompleteDelegate;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionCompleteDelegate;

protected:
	/**
	 * Internal callbacks for the delegate we'll add to the Online Session Interface delegate list.
	 * Thise don't need to be called outside this class.
	 */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	TWeakPtr<IOnlineSession> OnlineSessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	/**
	 * To add to the Online Session Interface delegate list.
	 * We'll bind our MultiplayerSessionSubsystem internal callbacks to these.
	 */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	bool bCreateSessionOnDestroy { false };
	int32 LastNumPublicConnections;
	FString LastMatchType;
	
};
