// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "MainPlayerController.h"
#include "UCppSaveGame.h"
#include "ItemStorage.h"

AMain::AMain()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (pulls towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; //camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true; //Rotate arm based on controller

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(48.f, 150.f);

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach camera to the end of the boom and let the boom adjust to match 
	// the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	// Set up turn rate
	BaseTurnRate = 65.f;
	BaseLookUpAtRate = 65.f;

	// dont rotate when the controller rotates
	// Let that just affect the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// configure Character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // ... at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	MaxHealth = 100.f;
	MaxStamina = 150.f;
	Health = 65.f;
	Stamina = 120.f;
	Coins = 0;
	HealthDrainAmount = 2.0f;

	RunningSpeed = 650.f;
	SprintingSpeed = 1000.f;
	bShiftKeyDown = false;
	bLMBDown = false;
	bESCDown = false;
	bMovingForward = false;
	bMovingRight = false;

	//Initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	bAttacking = false;
	InterpSpeed = 16.f;
	bInterpToEnemy = false;
	bHasCombatTarget = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());
	if (!MainPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Main.cpp: current PlayerController BP is not of class AMainPlayerController !"));
	}
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if we are dead, do not do all other Tick functionality after this check. We dont have to we are dead anyway
	if (MovementStatus == EMovementStatus::EMS_Dead) { return; }

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	// Stamina bar update logic -------------------------------------------------------------------------------
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal: //when we are running
		 // and when shift is pressed
		if (bShiftKeyDown)
		{
				// check, if our stamina is below minimum
				if (Stamina - DeltaStamina <= MinSprintStamina)
				{
					// if it is below minimum, and we press key still drain stamina
					SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
					Stamina -= DeltaStamina;
				}
				else // but when stamina is not below or equal to minimum drain it
				{
					Stamina -= DeltaStamina;
				}

				// if we are pressing any wsad moving buttons
				if (bMovingForward || bMovingRight)
				{
					// when shift key is down update movement status, bcos we are running
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else // if we are NOT pressing any wsad moving buttons
				{
				
					SetMovementStatus(EMovementStatus::EMS_Normal);

				}
		
		}
		// shift key not pressed
		else
		{
				// is our stamina at maximum value ?
				if (Stamina + DeltaStamina >= MaxStamina) {
					//stop incrementing, we have max stamina 
					Stamina = MaxStamina;
				}
				// but when stamina is not at max
				else
				{
					// stamina is then regaining
					Stamina += DeltaStamina;
				}
			// we are not pressing shift, so just make us run
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	// When stamina is at below MINIMUM
	case EStaminaStatus::ESS_BelowMinimum:
		// and when we are pressing shift
		if (bShiftKeyDown) 
		{
			// Check if the stamina is 0
			if (Stamina - DeltaStamina <= 0.f) 
			{
				// if so, we are EXHAUSTED
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0.f;
				// And we are forced to run insted sprint
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else // if not below 0
			{
				// still drain stamina (//till 0)
				Stamina -= DeltaStamina;
				// if we are pressing any wsad moving buttons
				if (bMovingForward || bMovingRight)
				{
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else // if we are NOT pressing any wsad moving buttons
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);

				}
			}
			
		}
		// and shift key not pressed
		else
		{
			// but when we reach minstamina
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				// change STAMINA status to normal (from minimum)
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				// and still replenish stamina
				Stamina += DeltaStamina;
			}
			else // slowly recover stamina
			{
				Stamina += DeltaStamina;
			}

			// we are not pressing so we should be walking
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}

		break;

	// when stamina is drained out
	case EStaminaStatus::ESS_Exhausted:
		// and we are still pressing shift
		if (bShiftKeyDown)
		{
			// stamina is not recovering, is still at 0
			Stamina = 0.f;
		}
		// we are not pressing shift
		else
		{
			// we are exhausted, so we cant sprint
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			// but we are recovering stamina
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;

		// when we are exhausted
	case EStaminaStatus::ESS_ExhaustedRecovering:

		// if stamina is over minimum stamina value
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			// to jest powy¿ej minimum i zmienia status z poni¿ej minimum
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}
		// when exhausted we are recovering
		else
		{
			// we are recovering
			Stamina += DeltaStamina;
		}
		// we are running
		SetMovementStatus(EMovementStatus::EMS_Normal);


		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Stamina status update logic in Main.cpp: default switch statement. Stamina will not be updated "));
		break;
	}

	if (bInterpToEnemy && CombatTarget)
	{
		// destination for character rotation toward enemy (it will snap him)
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		// return rotator that is at correct rotation position AT THIS FRAME to do smooth transition from current rotation to target rotation,
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);
		//  set actor rotation to interp, and in Tick it will "lerp"  it at each frame
		SetActorRotation(InterpRotation);
	}

	// update combat target with our enemy location
	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}
 
// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// call the custom Jump() function created in AMain (which then calls overriden Jump from ACharacter)
	// we do not have to override StopJumping, since we add our custom functionality only to Jump
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("Esc", EInputEvent::IE_Pressed, this, &AMain::ESCDown).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Esc", EInputEvent::IE_Released, this, &AMain::ESCUp).bExecuteWhenPaused = true;


	PlayerInputComponent->BindAction("LMB", EInputEvent::IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", EInputEvent::IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, & AMain::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, & AMain::LookUp);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpRate);
}

bool AMain::CanMove(float Value)
{
	if (MainPlayerController)
	{
		return   (Value != 0.0f) && (!bAttacking) && (MovementStatus != EMovementStatus::EMS_Dead) && (!MainPlayerController->bPauseMenuVisible);
	}
	return false;
}

void AMain::MoveForward(float Value)
{
	// each time this function is called in tick (and we re not pressing button (Value == 0.f)) set moving forawrd to false (again,we are NOT pressing move button)
	bMovingForward = false;

	if (CanMove(Value)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// but when pressing move button, update moving forward
		bMovingForward = true;

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMain::MoveRight(float Value)
{
	// each time this function is called in tick (and we re not pressing button (Value == 0.f)) set moving right to false (again, we are NOT pressing move button)
	bMovingRight = false;

	if (CanMove(Value)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// but when pressing move button, update moving right
		bMovingRight = true;

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMain::Turn(float Value)
{
	if (CanMove(Value)) {
		AddControllerYawInput(Value);
	}
}

void AMain::LookUp(float Value)
{

	if (CanMove(Value)) {
		AddControllerPitchInput(Value);
	}
}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LMBDown()
{
	bLMBDown = true;

	// // if we are dead and lmb is pressed, do not execute rest of this function (we are not going to be able to pickup weapons etc.)
	// // return; will exit function and nothing past it will be executed
	if (MovementStatus == EMovementStatus::EMS_Dead) { return; }

	// WEAPON EQUIPPING -----------------------------------------------------------------
	// if overlapped with weapon and if weapon cast succeed, equip overlapped weapon
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if (EquippedWeapon)
	{
		Attack();
	}

}

void AMain::LMBUp()
{
	bLMBDown = false;
}

void AMain::ESCDown()
{
	bESCDown = true;
	
	// when escape is pushed, show PauseMenu
	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
	
	
}

void AMain::ESCUp()
{
	bESCDown = false;
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	// prevents from crashing if no equipped weapon is found on character
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	
	EquippedWeapon = WeaponToSet;
}

void AMain::Attack()
{
	if (!bAttacking && MovementStatus != EMovementStatus::EMS_Dead)
	{
		bAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			// choose random animaton for attack
			int32 Selection = FMath::RandRange(0,1);
			switch (Selection)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;

			case 1:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;

			default:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;
			}				
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Main character BP: no combat montage reference asset selected !"));
		}
	}

}

void AMain::AttackEnd()
{
	bAttacking = false;
	// when we finish attacking, set false to Interpetation towards enemy
	SetInterpToEnemy(false);

	if (bLMBDown)
	{

		Attack();
	}
}

// // Check to see if taken damage will put us below 0 health,
// // If so, call Die() 
void AMain::DecrementHealth(float Amount)
{
	IncrementCoins(-2);
}

// // overriding function provided by AActor
float AMain::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	if (Health - DamageAmount <= 0.f) {
		Health -= DamageAmount;
		Die();

		if (DamageCauser)
		{
			// get enemy from current damage causer (coz it can be falling rock, death zone etc.)
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			// if it is Enemy_BP
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}

	}
	else
	{
		Health -= DamageAmount;
	
	}
	
	return DamageAmount;

}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMain::IncrementHealth(float Amount)
{
	if (Health + Amount >= MaxHealth) {
		Health = MaxHealth;
	}
	else
	{

		Health += Amount;
	}
}

void AMain::Die()
{
	if (MovementStatus == EMovementStatus::EMS_Dead){return;}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Main character BP: no combat montage reference selected !"));
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMain::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}


// // overriden from ue4 character
void AMain::Jump()
{
	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
}



void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}




void AMain::ShowPickupLocations()
{
	for (int32 i = 0; i < PickupLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.f, 8, FLinearColor::Green, 10.0f, 0.5f);
	}
}


void AMain::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);

	}

}


void  AMain::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

FRotator AMain::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	// construct rotator 
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

// // update combat target whem the first enemy is killed
void AMain::UpdateCombatTarget() 
{
	TArray<AActor*>OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	// if we are not overlapping any actor, remove enemy healthbars from view. Also, do not update combat target (since we do not overlap enemy combat sphere)
	if (OverlappingActors.Num() == 0) 
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();
	
		// go through each actor in array and see how colse he is. If then next actor is closer to Player, set it ast the Closest enemy. It will filter array to the closest enemy this way
		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
			
		}

		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}

		
}


void AMain::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();
		FName CurrentLevelName(*CurrentLevel);
		if (CurrentLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(World,LevelName);
		}
	}
}

void AMain::SaveGame()
{
	// // CrateSaveGameObject has parameter class, thats why we use StaticClass(), it returns pointer to class.
	// // then we cast it to get our UUCppSaveGame class and save it to variable
	UUCppSaveGame* SaveGameInstance =  Cast<UUCppSaveGame>(UGameplayStatics::CreateSaveGameObject(UUCppSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Coins = Coins;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	if (EquippedWeapon) {
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	// // Create save slot with name of player 
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void AMain::LoadGame(bool SetPosition)
{
	// // same as in SaveGame() we create instance of class UUCppSaveGame
	UUCppSaveGame* LoadGameInstance = Cast<UUCppSaveGame>(UGameplayStatics::CreateSaveGameObject(UUCppSaveGame::StaticClass()));

	LoadGameInstance = Cast<UUCppSaveGame>( UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	// // set Main character health variable with amount saved in UUCppSaveGame slot called PlayerName (which was defined in UUCppSaveGame constructor)
	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	Coins = LoadGameInstance->CharacterStats.Coins;


	if (WeaponStorage)
	{
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);

		if (Weapons) {
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
		

			AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
			WeaponToEquip->Equip(this);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Main character BP: Weapon Storage reference is not set !"));

	}
	
	if (SetPosition)
	{
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	}
	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
}

