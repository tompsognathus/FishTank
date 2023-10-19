// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementGrid.generated.h"

class USphereComponent;
class UMaterial;

UCLASS()
class FISHGAME_API AMovementGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovementGrid();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetWorldLocationFromGridIndex(int RowIdx, int ColumnIdx);

	int GetGridIndexFromGridCoordinates(int RowIdx, int ColumnIdx);
	int GetGridWidth() const { return NumColumns; }
	int GetGridHeight() const { return NumRows; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateGridLocationMarkers();
	bool SetUpGridLocationMarkers();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid | Player")
	int NumColumns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid | Player")
	int NumRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid | Player")
	UStaticMeshComponent* GridBoundaryMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid | Player")
	UStaticMesh* GridMarkerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid | Player")
	UMaterial* GridMarkerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid | Player")
	TArray<UStaticMeshComponent*> GridLocationMarkerMeshComponents = TArray<UStaticMeshComponent*>();



};
