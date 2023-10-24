// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnerComponent.generated.h"

class AItem;
class AMovementGrid;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISHGAME_API USpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnItem(TSubclassOf<AItem> Item, FIntVector2 SpawnGridCoordinates);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnDistance = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AItem> ItemToSpawn;

private:
	float TimeUntilNextSpawn = 1.f;
	AActor* ParentActor;
	AMovementGrid* MovementGrid;
};
