

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSession, Log, All);

// 为了在蓝图中也能绑定委托，所以使用动态多播委托
// 因为 FOnlineSessionSearchResult 不是 UClass，所以不能使用动态委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateComplete, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartComplete, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyComplete, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinComplete, const FString&, Address);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFindComplete, const TArray<FOnlineSessionSearchResult>& SearchResults);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();
	
	void CreateSession(int32 MaxPlayerNum, FString MatchType);
	void StartSession();
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& Session);
	void DestroySession();

	FUniqueNetIdRepl GetUniqueNetId() const;

	UPROPERTY(BlueprintAssignable)
	FOnCreateComplete OnCreateCompleteDelegates;
	UPROPERTY(BlueprintAssignable)
	FOnStartComplete OnStartCompleteDelegates;
	UPROPERTY(BlueprintAssignable)
	FOnDestroyComplete OnDestroyCompleteDelegates;

	/// @param Address 解析会话后得到的地址
	FOnJoinComplete OnJoinCompleteDelegates;
	
	FOnFindComplete OnFindCompleteDelegates;
	
protected:
	
	void OnCreateComplete(FName SessionName, bool bSuccess);
	void OnStartComplete(FName SessionName, bool bSuccess);
	void OnFindComplete(bool bSuccess);
	void OnJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroyComplete(FName SessionName, bool bSuccess);
	
private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings, ESPMode::ThreadSafe> SessionSettings;
	TSharedPtr<FOnlineSessionSearch, ESPMode::ThreadSafe> SessionSearch;

	int32 CurrentMaxPlayerNum;
	FString CurrentMatchType;
	bool bCreateAfterDestroy;
	
	FDelegateHandle CreateCompleteHandle;
	FDelegateHandle StartCompleteHandle;
	FDelegateHandle FindCompleteHandle;
	FDelegateHandle JoinCompleteHandle;
	FDelegateHandle DestroyCompleteHandle;
	
};
