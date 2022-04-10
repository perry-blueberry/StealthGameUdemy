// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAiGuard.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
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

	GuardState = EAiState::Idle;
	CurrentTargetPointIndex = 0;
}

// Called when the game starts or when spawned
void AFPSAiGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();

	if (const auto PossessingAiController = Cast<AAIController>(GetController()))
	{
		AiController = PossessingAiController;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get possessing AI controller"));
		Destroy();
	}
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
	if (Cast<AFPSCharacter>(SeenPawn))
	{
		if (const auto MyGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode()))
		{
			MyGameMode->CompleteMission(SeenPawn, false);
		}	
	}

	SetGuardState(EAiState::Alerted);
}

// ReSharper disable CppParameterMayBeConstPtrOrRef
void AFPSAiGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, const float Volume)
// ReSharper restore CppParameterMayBeConstPtrOrRef
{
	if (!NoiseInstigator || GuardState == EAiState::Alerted)
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

	AiController->PauseMove(AiController->GetCurrentMoveRequestID());
	SetGuardState(EAiState::Suspicious);
	FTimerDelegate SetGuardDelegate;
	SetGuardDelegate.BindLambda([this](){SetGuardState(EAiState::Idle);});
	// SetGuardDelegate.BindUFunction(this, FName("SetGuardState"), EPathFollowingStatus::Idle);//::CreateUObject( this, &AFPSAiGuard::SetGuardState, EPathFollowingStatus::Idle );
	GetWorldTimerManager().SetTimer(ResetOrientationTimerHandle, SetGuardDelegate, 3.0f, false);
}

void AFPSAiGuard::SetGuardState(const EAiState NewGuardState)
{
	if (NewGuardState == GuardState || GuardState == EAiState::Alerted)
	{
		return;
	}
	GuardState = NewGuardState;
	OnStateChanged(GuardState);
	if (GuardState == EAiState::Idle)
	{
		AiController->ResumeMove(AiController->GetCurrentMoveRequestID());	
	}
}

void AFPSAiGuard::ResetOrientation()
{
	if (GuardState == EAiState::Alerted)
	{
		return;
	}
	TargetRotation = OriginalRotation;
	SetGuardState(EAiState::Idle);
}

// Called every frame
void AFPSAiGuard::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 1);
	SetActorRotation(NewRotation);

	// if (const auto CurrentTime = GetWorld()->TimeSeconds; CurrentTime - LastTimePrinted > 2)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Actor rotation: %f ----------- TargetRotation: %f ------------------------- OriginalRotation: %f"),
	// 		   GetActorRotation().Yaw, TargetRotation.Yaw, OriginalRotation.Yaw);
	// 	LastTimePrinted = CurrentTime;
	// }

	// Moving guard
	if (TargetPoints.Num() >= 2)
	{
		if (const auto CurrentTime = GetWorld()->TimeSeconds; CurrentTime - LastTimePrinted > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor rotation: %f ----------- TargetRotation: %f ------------------------- OriginalRotation: %f"),
				   GetActorRotation().Yaw, TargetRotation.Yaw, OriginalRotation.Yaw);
			LastTimePrinted = CurrentTime;
		}
		// Patrol to new target
		if (GuardState == EAiState::Idle && AiController->GetMoveStatus() != EPathFollowingStatus::Moving)
		{
			CurrentTargetPointIndex = ++CurrentTargetPointIndex % TargetPoints.Num();
			const auto NewTarget = TargetPoints[CurrentTargetPointIndex];
			AiController->MoveToActor(NewTarget); //UNavigationSystemV1::SimpleMoveToActor(AiController, NewTarget);
			auto Direction = NewTarget->GetActorLocation() - GetActorLocation();
			Direction.Normalize();
			TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			TargetRotation.Pitch = GetActorRotation().Pitch;
			TargetRotation.Roll = GetActorRotation().Roll;
		}
	}
	// Stationary guard
	else
	{
		if (GetActorRotation().Equals(TargetRotation, 0.1) && !TargetRotation.Equals(OriginalRotation, 0.1) && !GetWorldTimerManager().IsTimerActive(ResetOrientationTimerHandle))
		{
			GetWorldTimerManager().SetTimer(ResetOrientationTimerHandle, this, &AFPSAiGuard::ResetOrientation, 1.0f);
		}
	}
}