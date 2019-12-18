// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController(){
	EnemyHealthBarPositionInViewport = FVector2D (0.f,0.f);
}


void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainPlayerController: no HUDOverlayAsset widget is set in BPs references!"));
	}


	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		// if it was succesfully created
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Enemy Healthbar was not created at begin play !"));

		}
		FVector2D Alignment = FVector2D(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainPlayerController: no EnemyHealthBar widget is set in BPs references!"));
	}


	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		// if it was succesfully created
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainPlayerController: no PauseMenu widget is set in BPs references!"));
	}
}

void AMainPlayerController::Tick(float DealtaTime)
{
	Super::Tick(DealtaTime);

	if (EnemyHealthBar)
	{
		// // x is horizontal in screen, starting at 0 in left top corner
		// // y is vertical starting at 0 in left top corner
		// // so bottom right corner is 1,1 in screen system coordinates
		FVector2D ScreenLocation = FVector2D(0.0f,0.0f);
		ProjectWorldLocationToScreen(EnemyLocation, ScreenLocation);
		ScreenLocation -= EnemyHealthBarPositionInViewport;
		 

		FVector2D SizeInViewport(300.f, 50.f);
		EnemyHealthBar->SetPositionInViewport(ScreenLocation);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

// // show healthbar of current Player enemy
void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
	//	UE_LOG(LogTemp, Error, TEXT("[%s] no EnemyHealthbar created. Called by DisplayEnemyHealthbar"), *(this->GetName());

	}
}

// // Hide healthbar of current Player enemy
void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeUI;
		SetInputMode( InputModeUI);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{

	if (PauseMenu)
	{
		FInputModeGameOnly InputModeUI;
		SetInputMode(InputModeUI);
		bShowMouseCursor = false;
		bPauseMenuVisible = false;
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();

	}
	else
	{
		DisplayPauseMenu();
	}


}
