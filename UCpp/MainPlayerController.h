// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"


UCLASS()
class UCPP_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();
	/* // reference to UMG asset in the editor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;


	/* // variable to hold widget after creating it*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	UUserWidget* HUDOverlay;

	/* // reference to enemy healthbar widget, for creating it*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	TSubclassOf<class UUserWidget> WEnemyHealthBar;

	/* // variable to hold widget after creating it*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	UUserWidget* EnemyHealthBar;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	TSubclassOf<class UUserWidget> WPauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	UUserWidget* PauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "@@@ Properties")
	/* // position of enemy healthbar from enemy centre in world*/
	FVector2D EnemyHealthBarPositionInViewport;

	bool bPauseMenuVisible;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "@@@ Main Player Controller")
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "@@@ Main Player Controller")
	void RemovePauseMenu();
	/* // checks if pause menu is open or not and flips bool */
	void TogglePauseMenu();

	bool bEnemyHealthBarVisible;

	/* // show healthbar of current enemy active for Player */
	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector EnemyLocation;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DealtaTime) override;
};
