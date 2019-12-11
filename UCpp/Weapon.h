// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup		UMETA(DisplayName = "Pickup"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_MAX			UMETA(DisplayName = "DefaultMAX") // default empty
};


UCLASS()
class UCPP_API AWeapon : public AItem
{
	GENERATED_BODY()
public:

	AWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "@@@ Weapon Setup")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ Weapon Setup")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Weapon Setup")
	bool bWeaponParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "@@@ Weapon Setup")
	class USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Weapon Setup")
	class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Weapon Setup")
	USoundCue* SwingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Weapon Setup")
	class UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Weapon Setup")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Weapon Setup")
	/* // Reference to ue4 provided damage type container */
	TSubclassOf<UDamageType>DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "@@@ Weapon Setup")
	/* // controller we pass In when we deal damage */
	AController* WeaponInstigator;

	/* // sets controller as WeaponInstigator*/
	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

protected:

	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	void Equip(class AMain* Char);

	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State;  }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;


	/* 
	//	ActivateCollision(); DeactivateCollision();
	//  Fired by AnimNotifies (which are executed in BPs, these generate event in Anim_BP)
	*/

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();
	
};
