// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	for (auto PlayerControllerIterator = GetWorld()->GetPlayerControllerIterator(); PlayerControllerIterator; ++PlayerControllerIterator)
	{
		APlayerController* PlayerController = PlayerControllerIterator->Get();
		if (const auto Pawn = PlayerController->GetPawn(); Pawn->IsLocallyControlled())
		{
			Pawn->DisableInput(PlayerController);
		}
	}
}
