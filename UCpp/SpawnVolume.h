// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class UCPP_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ SpawnVolume Properties")
	class UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ SpawnVolume Properties")
	TSubclassOf<AActor> Actor_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ SpawnVolume Properties")
	TSubclassOf<AActor> Actor_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ SpawnVolume Properties")
	TSubclassOf<AActor> Actor_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ SpawnVolume Properties")
	TSubclassOf<AActor> Actor_4;

	TArray <TSubclassOf<AActor>>SpawnArray;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "@@@ Spawn Volume Properties")
	/* // random point in box range */
	FVector GetSpawnPoint();

	UFUNCTION(BlueprintPure, Category = "@@@ Spawn Volume Properties")
	TSubclassOf<AActor> GetSpawnActor();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "@@@ Spawn Volume Properties")
	void SpawnOurActor(UClass* ToSpawn, const FVector& Location );
};
