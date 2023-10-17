// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementGrid.h"

// Sets default values
AMovementGrid::AMovementGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridBoundaryMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grid Boundary Mesh"));
	
	for (idx = 0; idx < GridWidth * GridHeight; idx++)
	{

	}

}

// Called when the game starts or when spawned
void AMovementGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovementGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

