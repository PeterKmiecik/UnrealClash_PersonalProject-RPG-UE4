// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	InitialLocation = FVector(0.f);
	PlacedLocation = FVector(0.f);
	bInitializeFloaterLocations = false;
	WorldOrigin = FVector(0.f, 0.f, 0.f);
	bShouldFloat = false;
	InitialDirection = FVector(0.f, 0.f, 0.f);
	InitialForce = FVector(500000.0f, 0.0f, 0.0f);
	RunningTime = 0.f;

	A = 0.f;
	B = 0.f;
	C = 0.f;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();
	
	float InitialX = FMath::FRandRange(-500.f,500.f);
	float InitialY = FMath::FRand();
	float InitialZ = FMath::FRand();

	InitialLocation.X = InitialX;
	InitialLocation.Y = InitialY;
	InitialLocation.Z = InitialZ;

	InitialLocation *= 2.f;


	PlacedLocation = GetActorLocation();
	if (bInitializeFloaterLocations) {
		SetActorLocation(InitialLocation);
	}
	
	
	/*StaticMeshComponent->AddForce(InitialForce, NAME_None, true);
	StaticMeshComponent->AddTorque(InitialForce);*/
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShouldFloat)
	{
		RunningTime += DeltaTime;
		
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = NewLocation.Z + A * FMath::Sin(B * RunningTime * C);

		SetActorLocation(NewLocation);
		
	}


}

