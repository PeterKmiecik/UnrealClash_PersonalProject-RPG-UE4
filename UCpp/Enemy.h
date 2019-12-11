// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
 enum class EEnemyMovementStatus : uint8
{
	/** EExx bcos EnumEnemy which uses Exx notation for enum records */

	EEMS_Idle			UMETA(DisplayName = "Idle"),
	EEMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EEMS_Attacking		UMETA(DisplayName = "Attacking"),
	EEMS_Dead			UMETA(DisplayName = "Dead"),
	EEMS_MAX			UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class UCPP_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	/** Sets default values for this character's properties */
	AEnemy();

	/* // If enemy does not have valid target, stop attacking (e.g. when Player is dead) */
	bool bHasValidTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ References")
	class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ References")
	USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ References")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; };

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ References")
	class AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	USoundCue* SwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	/* // provided by UE4, intended to define and describe a particular form of damage and to provide an avenue for customizing responses to damage from various sources. */
	TSubclassOf<UDamageType>DamageTypeClass;

	/* //  particles when being hit by player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "@@@ References")
	class UAnimMontage* SpiderCombatMontage;

	/* // Timer ref to handle enemy corpse disapperance after death*/
	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	/* // amount of delay for corpse dissaperance after enemy death*/
	float DeathDealy;

	/* // Randomizing Enemy Attacks -------------------------------------------------------------------------
	// @AttackMinTime  @AttackMaxTime used to dictate waiting time till next attack */

	/* // @AttackTimer - is timer for randomizing enemy wait time until attack, to make it less robotic */
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	float AttackMaxTime;
	/*------------------------------------------------------------------------- */

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;

	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ References")
	bool bOverlapingCombatSphere;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMain* Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ References")
	AMain* CombatTarget;

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	/*****************************************************************
	// @ ActivateCollision(); @ DeactivateCollision();
	// Are fired by AnimNotifies (which are executed in BPs, they generate event in Anim_BP)*/
	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();
	/*******************************************************************/

	UFUNCTION(BlueprintCallable)
	/* // to know when enemy was attacking */
	void AttackEnd();

	void Attack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ References") 
	/* // indicates if enemy is attacking player */
	bool bAttacking; 	


	/* // override of UE4 function */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/* // called when health <= 0*/
	void Die(AActor* CauserOfDeath);

	UFUNCTION(BlueprintCallable)
	/* // called from AnimBP by DeathEnd notify*/
	void DeathEnd();

	bool Alive();

	/* // makes enemy corpse disappear after death with some time delay */
	void Disappear();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ References")
	int32 CoinsRewardAfterHit;

	
};