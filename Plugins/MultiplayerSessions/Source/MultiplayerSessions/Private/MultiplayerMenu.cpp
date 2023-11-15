#include "MultiplayerMenu.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Engine.h"

bool UMultiplayerMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	
	HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostButtonClick);
	JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClick);
	
	return true;
}

void UMultiplayerMenu::OnCreateSessionComplete(bool bSuccess)
{
	if (!bSuccess) return;

	SwitchInputMode(true);
	
	GetWorld()->ServerTravel(LobbyURL);
}

void UMultiplayerMenu::OnFindSessionComplete(const TArray<FOnlineSessionSearchResult>& Results)
{
	if (!SessionsSubsystem.IsValid()) return; 
	
	for (auto Result: Results)
	{
		FString Value;
		Result.Session.SessionSettings.Get(FName("MatchType"), Value);

		if (Value == MatchType)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Cyan, Result.Session.OwningUserName);
			
			SessionsSubsystem->JoinSession(Result);
			return;
		}
	}
}

void UMultiplayerMenu::OnJoinSessionComplete(const FString& Address)
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			SwitchInputMode(true);
			PlayerController->ClientTravel(Address, TRAVEL_Absolute);
		}
	}
}

void UMultiplayerMenu::MenuSetup(const FString& LobbyPath)
{
	LobbyURL = LobbyPath + FString("?listen");
	
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	SwitchInputMode(false);

	SessionsSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>();

	SessionsSubsystem->OnCreateCompleteDelegates.AddDynamic(this, &ThisClass::OnCreateSessionComplete);
	SessionsSubsystem->OnJoinCompleteDelegates.AddDynamic(this, &ThisClass::OnJoinSessionComplete);
	SessionsSubsystem->OnFindCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionComplete);
}

void UMultiplayerMenu::OnHostButtonClick()
{
	SessionsSubsystem->CreateSession(4, MatchType);
}

void UMultiplayerMenu::OnJoinButtonClick()
{
	SessionsSubsystem->FindSessions(100000);
}

void UMultiplayerMenu::SwitchInputMode(bool bInputModeGame)
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (bInputModeGame)
			{
				FInputModeGameOnly GameModeData;
				PlayerController->SetInputMode(GameModeData);
				PlayerController->SetShowMouseCursor(false);
			}
			else
			{
				FInputModeUIOnly UIModeData;
				UIModeData.SetWidgetToFocus(TakeWidget());
				UIModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(UIModeData);
				PlayerController->SetShowMouseCursor(true);
			}
		}
	}
}
