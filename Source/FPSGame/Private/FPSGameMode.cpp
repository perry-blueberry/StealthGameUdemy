// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, const bool bMissionComplete)
{
	if (InstigatorPawn)
	{
		TArray<AActor*> Spectators;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, Spectators);
		if (const auto PlayerController = Cast<APlayerController>(InstigatorPawn->GetController()); PlayerController && Spectators.Num() > 0)
		{
			PlayerController->SetViewTarget(Spectators[0]);
		}
	}
	if (const auto GameState = GetGameState<AFPSGameState>())
	{
		GameState->MulticastOnMissionComplete(InstigatorPawn, bMissionComplete);
	}
	OnMissionCompleted(InstigatorPawn, bMissionComplete);
}
