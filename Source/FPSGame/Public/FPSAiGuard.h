// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "FPSAiGuard.generated.h"

class AAIController;

UENUM(BlueprintType)
enum class EAiState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
// ReSharper disable CppClassCanBeFinal
class FPSGAME_API AFPSAiGuard : public ACharacter
	// ReSharper restore CppClassCanBeFinal
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

	void SetGuardState(const EAiState NewGuardState);

	UPROPERTY(EditAnywhere, Category=AI)
	TArray<ATargetPoint*> TargetPoints;

	UPROPERTY()
	AAIController* AiController;

	FTimerHandle ResetOrientationTimerHandle;
	FRotator TargetRotation;
	FRotator OriginalRotation;


	float LastTimePrinted = 0;
	int CurrentTargetPointIndex;

	UPROPERTY(ReplicatedUsing=OnRep_GuardState, BlueprintReadWrite)
	EAiState GuardState;

	UFUNCTION()
	void OnRep_GuardState();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
