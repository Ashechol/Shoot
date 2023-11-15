// Created by Ashechol


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto PlayerNum = GameState.Get()->PlayerArray.Num();

	if (PlayerNum == 2)
	{
		if (auto World = GetWorld())
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("Game/Shoot/Maps/Battle?listen"));
		}
	}		
}
