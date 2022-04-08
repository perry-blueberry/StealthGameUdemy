// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAiGuard.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
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
	OriginalRotation = GetActorRotation();
}

void AFPSAiGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (!SeenPawn)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Seen pawn"));
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32, 12, FColor::Red, false, 10);
	// TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SeenPawn->GetActorLocation());
}

void AFPSAiGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (!NoiseInstigator)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Heard pawn %d"), Volume);
	DrawDebugSphere(GetWorld(), Location, 32, 12, FColor::Green, false, 10);

	UE_LOG(LogTemp, Warning, TEXT("ReSetting timer"));
	GetWorldTimerManager().ClearTimer(ResetOrientationTimerHandle);
	auto Direction = Location - GetActorLocation();
	Direction.Normalize();
	TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	TargetRotation.Pitch = GetActorRotation().Pitch;
	TargetRotation.Roll = GetActorRotation().Roll;
}

// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 1);
	SetActorRotation(NewRotation);

	const auto CurrentTime = GetWorld()->TimeSeconds;
	if (CurrentTime - LastTimePrinted > 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor rotation: %f ----------- TargetRotation: %f ------------------------- OriginalRotation: %f"),
			   GetActorRotation().Yaw, TargetRotation.Yaw, OriginalRotation.Yaw);
		LastTimePrinted = CurrentTime;
	}
	if (GetActorRotation().Equals(TargetRotation, 0.1) && !TargetRotation.Equals(OriginalRotation, 0.1) && !GetWorldTimerManager().IsTimerActive(ResetOrientationTimerHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting timer"));
		GetWorldTimerManager().SetTimer(ResetOrientationTimerHandle, this, &AFPSAiGuard::ResetOrientation, 1.0f);
	}
}

void AFPSAiGuard::ResetOrientation()
{
	TargetRotation = OriginalRotation;
}
