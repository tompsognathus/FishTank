// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementGrid.h"
#include "Components/SphereComponent.h"


// Sets default values
AMovementGrid::AMovementGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridBoundaryMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grid Boundary Mesh"));
	this->SetRootComponent(GridBoundaryMeshComponent);


	
}

// Called when the game starts or when spawned
void AMovementGrid::BeginPlay()
{
	Super::BeginPlay();

	CreateGridLocationMarkers();
	bool SetupSucceeded = SetUpGridLocationMarkers();
	if (!SetupSucceeded)
	{
		UE_LOG(LogTemp, Error, TEXT("AMovementGrid::BeginPlay: SetUpGridLocationMarkers failed."));
		return;
	}
}

// Called every frame
void AMovementGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AMovementGrid::GetWorldLocationFromGridIndex(int RowIdx, int ColumnIdx)
{
	if (RowIdx > NumRows)
	{
		UE_LOG(LogTemp, Error, TEXT("AMovementGrid::GetWorldLocationFromGridIndex: RowIdx out of bounds"));
		return FVector();
	}
	if (ColumnIdx > NumColumns)
	{
		UE_LOG(LogTemp, Error, TEXT("AMovementGrid::GetWorldLocationFromGridIndex: ColumnIdx out of bounds"));
		return FVector();
	}

	// Get the corresponding index in the GridLocationMarkerMeshComponents array
	int GridMarkerIdx = RowIdx * NumColumns + ColumnIdx;

	if (!IsValid(GridLocationMarkerMeshComponents[GridMarkerIdx]))
	{
		UE_LOG(LogTemp, Error, TEXT("AMovementGrid::GetWorldLocationFromGridIndex: Invalid GridLocationMarkerMeshComponents[GridMarkerIdx]"));
		return FVector();
	}

	FVector GridMarkerLocation = GridLocationMarkerMeshComponents[GridMarkerIdx]->GetComponentLocation();

	return GridMarkerLocation;
}


void AMovementGrid::CreateGridLocationMarkers()
{
	int NumGridMarkers = NumRows * NumColumns;
	for (int idx = 0; idx < NumGridMarkers; idx++)
	{
		FString GridMarkerName = TEXT("Grid Marker Mesh") + FString::FromInt(idx);
		UStaticMeshComponent* GridMarkerMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(*GridMarkerName));
		GridMarkerMeshComponent->RegisterComponent();
		GridMarkerMeshComponent->AttachToComponent(GridBoundaryMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

		GridLocationMarkerMeshComponents.Add(GridMarkerMeshComponent);

	}
}

bool AMovementGrid::SetUpGridLocationMarkers()
{
	UE_LOG(LogTemp, Error, TEXT("%d"), GridLocationMarkerMeshComponents.Num());
	if (GridLocationMarkerMeshComponents.Num() != NumRows * NumColumns)
	{
		UE_LOG(LogTemp, Error, TEXT("GridLocationMarkerMeshComponents.Num() != NumRows * NumColumns"));
		return false;
	}

	int NumGridMarkers = NumRows * NumColumns;
	for (int idx = 0; idx < NumGridMarkers; idx++)
	{
		int GridRow = idx / NumRows;
		int GridColumn = idx % NumColumns;

		if (!IsValid(GridLocationMarkerMeshComponents[idx]))
		{
			UE_LOG(LogTemp, Error, TEXT("AMovementGrid::BeginPlay: Invalid GridLocationMarkerMeshComponents[idx]"));
			return false;
		}
		GridLocationMarkerMeshComponents[idx]->SetRelativeLocation(FVector(0.0f, GridRow * 100.f / (NumRows - 1), GridColumn * 100.0f / (NumColumns - 1)));
		GridLocationMarkerMeshComponents[idx]->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
		if (!IsValid(GridMarkerMesh))
		{
			UE_LOG(LogTemp, Error, TEXT("AMovementGrid::BeginPlay: Invalid GridMarkerMesh"));
			return false;
		}
		GridLocationMarkerMeshComponents[idx]->SetStaticMesh(GridMarkerMesh);
		if (!IsValid(GridMarkerMaterial))
		{
			UE_LOG(LogTemp, Error, TEXT("AMovementGrid::BeginPlay: Invalid GridMarkerMaterial"));
			return false;
		}
		GridLocationMarkerMeshComponents[idx]->SetMaterial(0, GridMarkerMaterial);
	}

	return true;
}


