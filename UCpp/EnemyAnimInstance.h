// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCPP_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UEnemyAnimInstance();

	virtual void NativeInitializeAnimation() override;


	UFUNCTION(BlueprintCallable, Category = "@@@ Enemy Anim Instance")
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ References")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ References")
	class APawn* Pawn;

	/* reference to enemy that holds this anim instance */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ References")
	class AEnemy* Enemy;
};
