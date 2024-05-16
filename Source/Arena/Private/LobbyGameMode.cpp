// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState)
	{
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Red, FString::Printf(TEXT("Number of players: %d"), NumberOfPlayers));

			if (APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>())
			{
				FString PlayerName = PlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Player joined: %s"), *PlayerName));
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>())
	{
		FString PlayerName = PlayerState->GetPlayerName();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Player left: %s"), *PlayerName));
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Red, FString::Printf(TEXT("Number of players: %d"), NumberOfPlayers - 1));
		}
	}
}
