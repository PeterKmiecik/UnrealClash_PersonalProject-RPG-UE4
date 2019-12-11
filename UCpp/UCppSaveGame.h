// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UCppSaveGame.generated.h"


USTRUCT(BlueprintType)
struct FCharacterStats
{
	
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame data")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame data")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame data")
	float Stamina;

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame data")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame data")
	int32 Coins;

	/* // World location of character on map */
	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame data")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame data")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame data")
	FString WeaponName;

};

UCLASS()
class UCPP_API UUCppSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UUCppSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame")
	/* // Reference to struct with savegame variables*/
	FCharacterStats CharacterStats;

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "@@@ SaveGame")
	uint32 UserIndex;
};
