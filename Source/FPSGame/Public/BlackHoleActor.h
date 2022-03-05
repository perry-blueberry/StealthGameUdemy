// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHoleActor.generated.h"

UCLASS()
class FPSGAME_API ABlackHoleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHoleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Visual)
	class USceneComponent* NewRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Visual)
	class UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Collision)
	class USphereComponent* InnerSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Collision)
	class USphereComponent* OuterSphereComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
