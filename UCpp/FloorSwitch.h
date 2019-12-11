// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class UCPP_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();
	
	/* Overlap volume for functionality to be triggered*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class UBoxComponent* TriggerBox;

	/* switch for the charactert to step on*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class UStaticMeshComponent* FloorSwitch;

	/* Door to move when the floor switch is stepped on*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	UStaticMeshComponent* Door;

	/* Initial location for the door*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	FVector InitialDoorLocation;

	/* Initial location for the floor switch */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	FVector InitialSwitchLocation;

	FTimerHandle SwitchHandle;

	void CloseDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	float SwitchTime;

	bool bCharacterOnSwitch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "@@@ Floor Switch")
	void RaiseDoor();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "@@@ Floor Switch")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "@@@ Floor Switch")
	void RaiseFloorSwitch();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "@@@ Floor Switch")
	void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable, Category = "@@@ Floor Switch")
	void UpdateDoorLocation(float Z);

	UFUNCTION(BlueprintCallable, Category = "@@@ Floor Switch")
	void UpdateFloorSwitchLocation(float Z);
};
