// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class UCPP_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ References")
	/* Mesh for the platform*/
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "@@@ References")
	FVector StartPoint;

	UPROPERTY(EditAnywhere, meta=(MakeEditWidget = "true"), Category = "@@@ References")
	FVector EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ References")

	float InterpSpeed;

	FTimerHandle InterpTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ References")
	bool bInterping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ References")
	float InterpTime;

	float Distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void ToggleInterping();

	void SwapVectors(FVector& VecOne, FVector& VecTwo);

};
