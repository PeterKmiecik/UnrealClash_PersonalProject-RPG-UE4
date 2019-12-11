// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIController.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	// check if actors ale valid and add them to array
	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// // random location in box range
FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return Point;
}

// // gives random actor from SpawnArray
TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if (SpawnArray.Num() > 0)
	{
		// //we put to Selection number of elements of SpawnArray. -1 is bcos number of elements in array is 4 but we cant index number 4, arrays start at 0
		int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);
		return SpawnArray[Selection];
	}
	else 
	{
		return nullptr;
		UE_LOG(LogTemp, Error, TEXT("SpawnVolume: no actors selected to spawn in blueprint references."));
	}
}



void  ASpawnVolume::SpawnOurActor_Implementation(UClass* ToSpawn, const FVector& Location)
{ 
	FActorSpawnParameters SpawnParams;
	if (ToSpawn) 
	{
		UWorld* World = GetWorld();
		
		if (World)
		{
		 AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.0f), SpawnParams);
		 AEnemy* Enemy = Cast<AEnemy>(Actor);

		 if (Enemy)
		 {
			 Enemy->SpawnDefaultController();
			 AAIController* AICont = Cast<AAIController>(Enemy->GetController());
			 if (AICont)
			 {
				 Enemy->AIController = AICont;
			 }
		 }
		}
	}
}

