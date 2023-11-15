/// Created by Ashechol
/// 2023-10-28

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerMenu.generated.h"

class UButton;
class FOnlineSessionSearchResult;
class UMultiplayerSessionsSubsystem;

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Multiplayer Sessions")
	void MenuSetup(const FString& LobbyPath);

protected:

	virtual bool Initialize() override;

	UFUNCTION()
	virtual void OnCreateSessionComplete(bool bSuccess);
	UFUNCTION()
	virtual void OnJoinSessionComplete(const FString& Address);
	
	virtual void OnFindSessionComplete(const TArray<FOnlineSessionSearchResult>& Results);

private:
	
	TWeakObjectPtr<UMultiplayerSessionsSubsystem> SessionsSubsystem;

	FString LobbyURL = "";
	FString MatchType{"DeathMatch"};

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void OnHostButtonClick();

	UFUNCTION()
	void OnJoinButtonClick();

	void SwitchInputMode(bool bInputModeGame);
};
