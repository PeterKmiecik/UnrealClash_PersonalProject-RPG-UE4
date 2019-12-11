// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

UEnemyAnimInstance::UEnemyAnimInstance(){
}


void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AEnemy>(Pawn);

		}
	}

}

// // updating base variables for this anim BP class
void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Enemy == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
		

			Enemy = Cast<AEnemy>(Pawn);

		}

	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}

}
