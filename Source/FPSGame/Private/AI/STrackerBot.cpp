// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	MovementForce = 1000;
	RequiredDistanceToTarget = 100;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	NextPathPoint = GetNextPathPoint();
}

FVector ASTrackerBot::GetNextPathPoint()
{
	const auto NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (auto PathPoints = NavigationPath->PathPoints; PathPoints.Num() > 1)
	{
		return PathPoints[1];
	}
	return GetActorLocation();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// const auto DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
	
	if (!GetActorLocation().Equals(NextPathPoint, RequiredDistanceToTarget))
	{
		auto ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= MovementForce;
		MeshComponent->AddForce(ForceDirection);
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 20, FColor::Red, false, 2, 1);
	}
	else
	{
		NextPathPoint = GetNextPathPoint();
	}
	DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 4, 1);
}

