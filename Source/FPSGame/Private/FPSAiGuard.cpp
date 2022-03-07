// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAiGuard.h"

#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AFPSAiGuard::AFPSAiGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAiGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAiGuard::OnNoiseHeard);
}

// Called when the game starts or when spawned
void AFPSAiGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSAiGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (!SeenPawn)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Seen pawn"));
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32, 12, FColor::Red, false, 10);
}

void AFPSAiGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (!NoiseInstigator)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Heard pawn %d"), Volume);
	DrawDebugSphere(GetWorld(), Location, 32, 12, FColor::Green, false, 10);
}

// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
