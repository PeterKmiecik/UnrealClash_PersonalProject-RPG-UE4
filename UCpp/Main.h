// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal		UMETA(DisplayName = "Normal"),
	EMS_Sprinting	UMETA(DisplayName = "Sprinting"),
	EMS_Dead		UMETA(DisplayName = "Dead"),

	EMS_MAX			UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal					UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum			UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted				UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering		UMETA(DisplayName = "ExhaustedRecovering"),

	EMS_MAX						UMETA(DisplayName = "DefaultMAX")
};

/* Player character */
UCLASS()
class UCPP_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	UPROPERTY(EditDefaultsOnly, Category = "@@@ Properties")
	TSubclassOf<class AItemStorage> WeaponStorage;

	/* // store location of enemy we are fighting with */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	FVector CombatTargetLocation;

	/* // stores locations where pickups have been picked up */
	TArray<FVector> PickupLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "@@@ Properties")
	class AWeapon* EquippedWeapon;

	UFUNCTION(BlueprintCallable, Category = "@@@ Main character")
	void ShowPickupLocations();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	float MinSprintStamina;

	/*   ---------------------------------------- ---------------------------------------- ---------------------------------------- */
	/* rotating player character towards enemy functionality 
	references for showing enemy healthbar to player while in combat with enemy*/

	float InterpSpeed;
	bool bInterpToEnemy; 
	void SetInterpToEnemy(bool Interp);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	/* // @Param CombatTarget is actual player enemy, which is overlapping with enemy combat sphere */
	class AEnemy* CombatTarget;

	void UpdateCombatTarget();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	bool bHasCombatTarget;

	/* // for bHasCombatTarget which is used in showing enemy healthbar to player while in combat with enemy*/
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	/* // Sets player as actual enemy which is overlapping with enemy combat sphere */
	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

	/* // Take Main character Z rotation toward facing enemy (get what rotation we need to look toawrd target)*/
	FRotator GetLookAtRotationYaw(FVector Target);

	/* ---------------------------------------- ---------------------------------------- ---------------------------------------- */

	/* // Set movement Status and running speed */
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	float SprintingSpeed;

	bool bShiftKeyDown;

	/* // pressed down to enable sprinting*/
	void ShiftKeyDown();

	/* // updated when keys W,S are pressed */
	bool bMovingForward;

	/* // released down to stop sprinting */
	void ShiftKeyUp();

	/* // updated when keys A,D are pressed */
	bool bMovingRight;

	bool CanMove(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties", meta=(AllowPrivateAccess = "true"))
	/* // Camera boom positioning camera behind player */
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties", meta = (AllowPrivateAccess = "true"))
	/* // camera behind player */
	class UCameraComponent* FollowCamera;
	
	/* // Base turn rates to scale turning for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	float BaseLookUpAtRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	class USoundCue* HitSound;

	/* Player Stats ---------------------------------------------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "@@@ Properties")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	float Stamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "@@@ Properties")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	int32 Coins;

	void Die();

	virtual void Jump() override;

	/* // Check to see if taken damage will put us below 0 health, If so, call Die() */
	void DecrementHealth(float Amount);

	/* // override of UE4 function */
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "@@@ Main character")
	/* // after picking coins pickup*/
	void IncrementCoins(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "@@@ Main character")
	/* // after picking a health pickup*/
	void IncrementHealth(float Amount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* // Called for forwards/backwards input */
	void MoveForward(float Value);

	/* // Called for side to side input */
	void MoveRight(float Value);

	/* // Called for Yaw rotation*/
	void Turn(float Value);

	/* // Called for pitch rotation */
	void LookUp(float Value);

	/* // Called via input to turn at a given rate
	// @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate */
	void TurnAtRate(float Rate);
	
	/* // Called via input to look Up/Down at a given rate
	// @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired look Up/Down rate */
	void LookUpRate(float Rate);

	bool bLMBDown;

	void LMBDown();

	void LMBUp();


	bool bESCDown;

	void ESCDown();

	void ESCUp();


	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; };
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; };

	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Properties")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "@@@ Properties")
	class UAnimMontage* CombatMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	/*  // for filtering array of actors which are overlapping as CombatTargets
		// filters out everything that IS a Enemy or DERIVES from AEnemy */
	TSubclassOf<AEnemy>EnemyFilter;

	void SwitchLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	float HealthDrainAmount;
};
