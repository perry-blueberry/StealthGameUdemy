// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, const bool bMissionSuccess)
{
	for (auto PlayerControllerIterator = GetWorld()->GetPlayerControllerIterator(); PlayerControllerIterator; ++PlayerControllerIterator)
	{
		if (AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(PlayerControllerIterator->Get()); PlayerController && PlayerController->IsLocalController())
		{
			PlayerController->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

			if (const auto Pawn = PlayerController->GetPawn())
			{
				Pawn->DisableInput(PlayerController);
			}
		}
	}
}
