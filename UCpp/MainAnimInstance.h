// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

UCLASS()
class UCPP_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "@@@ Main Anim Instance")
	/* // updates basic variables for this AnimationBP asset*/
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	class APawn* Pawn;

	/* // reference to Main character class that holds this anim instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	class AMain* Main;


};
