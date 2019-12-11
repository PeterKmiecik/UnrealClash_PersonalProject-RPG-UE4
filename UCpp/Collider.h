// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class UCPP_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "@@@ References")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "@@@ References")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	float MovementSpeedMultiplier;
	
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh) { MeshComponent = Mesh; }
	FORCEINLINE USphereComponent* GetSphereComponent() { return SphereComponent; }
	FORCEINLINE void SetSphereComponent(USphereComponent* Sphere) { SphereComponent = Sphere; }
	
	UPROPERTY(VisibleAnywhere, Category = "@@@ References")
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, Category = "@@@ References")
	class USpringArmComponent * SpringArm;

	FORCEINLINE UCameraComponent* GetCameraComponent() { return Camera; }
	FORCEINLINE void SetCameraComponent(UCameraComponent* InCamera) { Camera = InCamera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return SpringArm; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* InSpringArm) { SpringArm = InSpringArm; }

	UPROPERTY(VisibleAnywhere, Category = "@@@ References")
	class UColliderMovementComponent* OurMovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

private:
	void MoveForward(float input);
	void MoveRight(float input);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	FVector2D CameraInput;
};
