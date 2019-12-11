// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class UCPP_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();
	//Location used by set Actor location when the BeginPlay() is called
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "@@@ References")
		FVector InitialLocation = FVector(0.0f);

	// Location of Actor when dragged in from the editor
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "@@@ References")
	FVector PlacedLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "@@@ References")
	FVector WorldOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ References")
	FVector InitialDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "@@@ References")
	bool bInitializeFloaterLocations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "@@@ References")
	bool bShouldFloat;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "@@@ References")
	FVector InitialForce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Comment */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "ActorMeshComponents")
		UStaticMeshComponent* StaticMeshComponent;

private:

	float RunningTime;

	UPROPERTY(EditAnywhere, Category = "@@@ References")
	float A;
	UPROPERTY(EditAnywhere,  Category = "@@@ References")
	float B;
	UPROPERTY(EditAnywhere,  Category = "@@@ References")
	float C;
};
