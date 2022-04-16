// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"

#include "FPSCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SetReplicates(true);
	// SphereComponent->OnComponentBeginOverlap.Add()
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSObjectiveActor::PlayEffects() const
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFx, GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	if (GetLocalRole() == ROLE_Authority)
	{
		if (AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor))
		{
			MyCharacter->bIsCarryingObjective = true;
			Destroy();
		}
	}
}

