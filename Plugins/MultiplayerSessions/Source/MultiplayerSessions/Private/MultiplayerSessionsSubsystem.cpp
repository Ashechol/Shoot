#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

DEFINE_LOG_CATEGORY(LogSession)

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();

		if (!SessionInterface.IsValid())
		{
			UE_LOG(LogSession, Error, TEXT("Cannot get session interface!"))
		}
	}
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 MaxPlayerNum, FString MatchType)
{
	CurrentMaxPlayerNum = MaxPlayerNum;
	CurrentMatchType = MatchType;

	if (SessionInterface->GetNamedSession(NAME_GameSession))
	{
		bCreateAfterDestroy = true;
		DestroySession();
		return;
	}
	
	CreateCompleteHandle =
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateComplete);

	// Session Setting
	SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->NumPublicConnections = MaxPlayerNum;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	// SessionSettings->BuildUniqueId = GetBuildUniqueId();
	SessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// Steam 实现的 CreateSession 当会话正在创建或者创建成功都会返回 true
	if (!SessionInterface->CreateSession(*GetUniqueNetId(), NAME_GameSession, *SessionSettings))
	{
		SessionInterface->OnCreateSessionCompleteDelegates.Remove(CreateCompleteHandle);
	}
}	

void UMultiplayerSessionsSubsystem::OnCreateComplete(FName SessionName, bool bSuccess)
{
	if (!bSuccess)
	{
		UE_LOG(LogSession, Error, TEXT("%s creation failed"), *SessionName.ToString());
		return;
	}

	UE_LOG(LogSession, Log, TEXT("%s creation succeeded"), *SessionName.ToString());
	
	SessionInterface->OnCreateSessionCompleteDelegates.Remove(CreateCompleteHandle);

	OnCreateCompleteDelegates.Broadcast(bSuccess);
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	FindCompleteHandle =
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindComplete);
	
	// Search Settings
	SessionSearch = MakeShareable(new FOnlineSessionSearch);
	SessionSearch->MaxSearchResults = MaxSearchResults;
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	if (!SessionInterface->FindSessions(*GetUniqueNetId(), SessionSearch.ToSharedRef()))
	{
		SessionInterface->OnFindSessionsCompleteDelegates.Remove(FindCompleteHandle);

		OnFindCompleteDelegates.Broadcast(TArray<FOnlineSessionSearchResult>());
	}
	
}

void UMultiplayerSessionsSubsystem::OnFindComplete(bool bSuccess)
{
	SessionInterface->OnFindSessionsCompleteDelegates.Remove(FindCompleteHandle);

	if (SessionSearch->SearchResults.Num() <= 0)
		OnFindCompleteDelegates.Broadcast(TArray<FOnlineSessionSearchResult>());
	else
		OnFindCompleteDelegates.Broadcast(SessionSearch->SearchResults);
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& Session)
{
	JoinCompleteHandle =
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinComplete);

	if (!SessionInterface->JoinSession(*GetUniqueNetId(), NAME_GameSession, Session))
	{
		SessionInterface->OnJoinSessionCompleteDelegates.Remove(JoinCompleteHandle);
	}
}

void UMultiplayerSessionsSubsystem::OnJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Success) return;

	SessionInterface->OnJoinSessionCompleteDelegates.Remove(JoinCompleteHandle);
	
	FString Address;
	SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
	OnJoinCompleteDelegates.Broadcast(Address);
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	DestroyCompleteHandle =
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroyComplete);
	
	SessionInterface->DestroySession(NAME_GameSession);
}

FUniqueNetIdRepl UMultiplayerSessionsSubsystem::GetUniqueNetId() const
{
	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (LocalPlayer->CachedUniqueNetId.IsValid())
		return LocalPlayer->CachedUniqueNetId;

	return LocalPlayer->GetUniqueNetIdForPlatformUser();
}

void UMultiplayerSessionsSubsystem::OnStartComplete(FName SessionName, bool bSuccess)
{
}

void UMultiplayerSessionsSubsystem::OnDestroyComplete(FName SessionName, bool bSuccess)
{
	SessionInterface->OnDestroySessionCompleteDelegates.Remove(DestroyCompleteHandle);

	if (bCreateAfterDestroy)
		CreateSession(CurrentMaxPlayerNum, CurrentMatchType);

	OnDestroyCompleteDelegates.Broadcast(bSuccess);
}



