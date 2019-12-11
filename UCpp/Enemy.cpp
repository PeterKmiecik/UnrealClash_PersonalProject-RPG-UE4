// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"
#include "Weapon.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	// all characters have sphere as Root, so we can not make sphere a new root. We can only attach to it.
	AgroSphere->SetupAttachment(GetRootComponent());

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	// all characters have sphere as Root, so we can not make sphere a new root. We can only attach to it.
	CombatSphere->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(), FName("EnemySocket"));
	// attach to component in constructor generates Errors, use SetupAttachment() instead !
	// attach to component can be used in BeginPlay
	// CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "EnemySocket");

	AgroSphere->InitSphereRadius(600.f);
	CombatSphere->InitSphereRadius(75.f);
	bOverlapingCombatSphere = false;

	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 10.f;
	CoinsRewardAfterHit = 1;

	AttackMaxTime = 0.5f;
	AttackMinTime = 3.5f;
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;
	DeathDealy = 3.f;
	bHasValidTarget = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());

	// bind overlap events to overlap functions which are containing gameplay logic
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if the other overlapped actor is valid
	if (OtherActor && Alive())
	{
		// check if it is main character
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		// check if it is main character
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bHasValidTarget = false;

			// when ending overlap if player CombatTarget is this instance of enemy, reset CombatTarget so player no longer faces this instance of enemy
			if (Main->CombatTarget == this)
			{
				Main->SetCombatTarget(nullptr);
			}

			Main->SetHasCombatTarget(false);
			
			//when we end overlapping aggro sphere meanig out of combat, hide enemy health bar
			Main->UpdateCombatTarget();
			

			SetEnemyMovementStatus(EEnemyMovementStatus::EEMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if the other overlapped actor is valid
	if (OtherActor && Alive())
	{
		// check if it is main character
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bHasValidTarget = true;
			// when close to enemy interpolate Main character toward this enemy
			Main->SetCombatTarget(this);
			Main->SetHasCombatTarget(true);
			
			Main->UpdateCombatTarget();

			CombatTarget = Main;
			bOverlapingCombatSphere = true;
			// make random attack time delay based on min max
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			// Attack() function is called by Timer with random time provided 
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// if the other overlapped actor is valid
	if (OtherActor && OtherComp)
	{
		// check if it is main character
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			// @Param when not close to enemy, dont interpolate Main character	
			bOverlapingCombatSphere = false;

			MoveToTarget(Main);
			
			CombatTarget = nullptr;

			if (Main->CombatTarget == this)
			{
				Main->SetCombatTarget(nullptr);
				Main->bHasCombatTarget = false;
				Main->UpdateCombatTarget();
			}

			if (Main->MainPlayerController)
			{
				USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
				if (MainMesh)
				{
					Main->MainPlayerController->RemoveEnemyHealthBar();
				}
			}
			// reset attack random timer, after player leaves combat sphere, so enemy stop attack anim
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}


void AEnemy::MoveToTarget(AMain* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EEMS_MoveToTarget);
	if (AIController)
	{
		/// MoveTo function call and all prerequisites 
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(5.0f);

		FNavPathSharedPtr NavPath;

		// &NavPath is out parameter meaning, that it will be filled with data
		AIController->MoveTo(MoveRequest, &NavPath);
		
		/* Draw debug spheres based on AI NavPath points generated to character
		
		// we set Array of PathPoints for ... TODO: comment
		// other way to set Array is with using auto keyword instead of TArray<FNavPathPoint>
		TArray<FNavPathPoint> PathPoints =  NavPath->GetPathPoints();

		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.0f, 5.f);
		}
		*/
	}
}

// // Damage implementation here. Overlaping combat volumes generate this call
void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			if (Main->HitParticles)
			{
				
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
				if (TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0.f), false);
				}
				if (Main->HitSound)
				{
					UGameplayStatics::PlaySound2D(this, Main->HitSound);

				}

				if (DamageTypeClass)
				{
					Main->DecrementHealth(UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass));
					
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Enemy with name: %s DamageTypeClass reference is not set in BP!"), *this->GetName());

				}
			}
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void AEnemy::ActivateCollision()
{
	 CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEnemy::Attack()
{
	if (Alive() && bHasValidTarget)
	{
			
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EEMS_Attacking);
		}
		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance) {

				AnimInstance->Montage_Play(SpiderCombatMontage, 1.35f);
				// play desired montage section
				AnimInstance->Montage_JumpToSection(FName("Attack"), SpiderCombatMontage);

			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Enemys %s reference for Mesh or CombatMontage is not set in BP ! Caller: AEnemy:Attack() "), *this->GetName());

			}
			if (SwingSound)
			{
				UGameplayStatics::PlaySound2D(this, SwingSound);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemys %s reference for SwingSound is not set in BP! "), *this->GetName());
			}
		}
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	// but if enemy is still overlapping player combat Sphere
	if (bOverlapingCombatSphere)
	{
		// then still attack player
		// make random attack time delay based on min max
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		// Attack() function is called by Timer with random time provided 
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
//AWeapon* Weapon = Cast<AWeapon>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetOwner());
	
	if (CombatTarget)
	{
		CombatTarget->IncrementCoins(CoinsRewardAfterHit);
		CombatTarget->IncrementHealth(CombatTarget->HealthDrainAmount);
	}

	if (Health - DamageAmount <= 0.0f)
	{
		Health -= DamageAmount;
		Die(DamageCauser);
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AEnemy::Die(AActor* CauserOfDeath)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {

		SetEnemyMovementStatus(EEnemyMovementStatus::EEMS_Dead);
		AnimInstance->Montage_Play(SpiderCombatMontage, 1.35f);
		// play desired montage section
		AnimInstance->Montage_JumpToSection(FName("Death"), SpiderCombatMontage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Enemys %s reference for Mesh or CombatMontage is not set in BP ! Caller: AEnemy:Attack() "), *this->GetName());
	}

	// turn off all collisions, so no logic will trigger
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bAttacking = false;

	// after first enemy death update combat target for the next enemy
	AMain* Main = Cast<AMain>(CauserOfDeath);
	if (Main)
	{
		Main->UpdateCombatTarget();
	}
}

// // called from AnimBP with AnimNotify_DeathEnd at the end of death animation section
void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDealy);
	
}
// // true if enemy is alive, false if dead
// // use for checks, when You do not want to go to any function execution when enemy is dead
bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EEMS_Dead;

}
/* // calls Destroy() (for garbage collection) makes enemy corpse disappear after death with some time delay */
void AEnemy::Disappear()
{
	Destroy();
}
