 // Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleActor.h"

#include "Components/SphereComponent.h"

 // Sets default values
ABlackHoleActor::ABlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("NewRootComponent"));
	RootComponent = NewRootComponent;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);

	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComponent"));
	InnerSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerSphereComponent->SetupAttachment(RootComponent);
	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComponent"));
	OuterSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OuterSphereComponent->SetupAttachment(RootComponent);
	OuterSphereComponent->SetSphereRadius(200);

}

// Called when the game starts or when spawned
void ABlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	TArray<UPrimitiveComponent*> InnerOverlappingComponents;
	InnerSphereComponent->GetOverlappingComponents(InnerOverlappingComponents);
	for (const auto Component : InnerOverlappingComponents)
	{
		if (auto OverlappingMeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			OverlappingMeshComponent->DestroyComponent();
		}
	}

	TArray<UPrimitiveComponent*> OuterOverlappingComponents;
	OuterSphereComponent->GetOverlappingComponents(OuterOverlappingComponents);
	for (const auto Component : OuterOverlappingComponents)
	{
		if (auto OverlappingMeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			OverlappingMeshComponent->AddImpulse((GetActorLocation() - OverlappingMeshComponent->GetComponentLocation()) * 3000 * DeltaTime);
		}
		
	}

}

