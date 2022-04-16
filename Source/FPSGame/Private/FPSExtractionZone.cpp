// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"

#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.f));
	RootComponent = OverlapComp;
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = {200.f, 200.f, 200.f};
	DecalComp->SetupAttachment(OverlapComp);
	DecalComp->SetRelativeRotation({90.f, 0, 0.f});
	
}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	
	Super::BeginPlay();
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == nullptr)
	{
		return;
	}
	if (MyPawn->bIsCarryingObjective)
	{
		if (const auto MyGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode()))
		{
			MyGameMode->CompleteMission(MyPawn, true);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
	UE_LOG(LogTemp, Warning, TEXT("Overlapped extraction zone"));
}

// Called every frame
void AFPSExtractionZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

