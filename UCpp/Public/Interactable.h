// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class UCPP_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category ="@@@ Properties")
	class UBoxComponent* OverlapBox;

	UPROPERTY(VisibleDefaultsOnly, Category = "@@@ Properties")
	class UBillboardComponent* Billboard ;

	UPROPERTY(BlueprintReadWrite, Category = "@@@ Interactable")
	class UStaticMeshComponent* VisualMesh;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Interactable", meta = (MakeEditWidget))
	FVector TransformWidget;


public:
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
