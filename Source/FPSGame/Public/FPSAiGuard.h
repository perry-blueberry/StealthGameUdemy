// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAiGuard.generated.h"

UENUM(BlueprintType)
enum class EAiState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAiGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAiGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnPawnSeen(class APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetOrientation();

	UFUNCTION(BlueprintImplementableEvent, Category=AI)
	void OnStateChanged(EAiState NewState);

	UPROPERTY(VisibleAnywhere, Category=Components)
	class UPawnSensingComponent* PawnSensingComp;

	FTimerHandle ResetOrientationTimerHandle;
	FRotator TargetRotation;
	FRotator OriginalRotation;

	float LastTimePrinted = 0;

	EAiState GuardState;
public:
	void SetGuardState(const EAiState NewGuardState);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
