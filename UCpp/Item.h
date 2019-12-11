// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

/* // Base class for all spawnable and pickable by Player objects*/

UCLASS()
class UCPP_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	/* base shape collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class UParticleSystemComponent* IdleParticlesComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class UParticleSystem* OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ References")
	class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	float RotationRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
