// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionComplete)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		TArray<AActor*> Spectators;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, Spectators);
		auto PlayerController = Cast<APlayerController>(InstigatorPawn->GetController());
		if (PlayerController && Spectators.Num() > 0)
		{
			PlayerController->SetViewTarget(Spectators[0]);
		}
	}
	OnMissionCompleted(InstigatorPawn, bMissionComplete);
}
