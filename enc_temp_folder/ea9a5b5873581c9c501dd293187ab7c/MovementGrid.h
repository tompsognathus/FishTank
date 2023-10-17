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

	FVector GetWorldLocationFromGridIndex(int RowIndex, int ColumnIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateGridLocationMarkers();
	bool SetUpGridLocationMarkers();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid")
	int NumColumns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid")
	int NumRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid")
	UStaticMeshComponent* GridBoundaryMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid")
	UStaticMesh* GridMarkerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid")
	UMaterial* GridMarkerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Grid")
	TArray<UStaticMeshComponent*> GridLocationMarkerMeshComponents = TArray<UStaticMeshComponent*>();


};
