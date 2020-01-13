// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Main.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	//OverlapBox->SetupAttachment(GetRootComponent());
	RootComponent = OverlapBox;
	OverlapBox->SetVisibility(true);
	OverlapBox->bHiddenInGame = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	Billboard->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnOverlapEnd);
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractable::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	if (OtherActor)
	{
		AMain * Main = Cast<AMain>(OtherActor);
			if (Main)
			{
				UE_LOG(LogTemp, Warning, TEXT("OVERLAPPED"));

			}
	}
}

void AInteractable::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

