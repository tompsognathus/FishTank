// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerComponent.h"
#include "Item.h"
#include "MovementGrid.h"

// Sets default values for this component's properties
USpawnerComponent::USpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	ParentActor = GetOwner();
	if (!IsValid(ParentActor))
	{
		UE_LOG(LogTemp, Error, TEXT("USpawnerComponent::BeginPlay: Invalid ParentActor"));
		return;
	}
	MovementGrid = Cast<AMovementGrid>(ParentActor);
	if (!IsValid(MovementGrid))
	{
		UE_LOG(LogTemp, Error, TEXT("USpawnerComponent::BeginPlay: Invalid MovementGrid"));
		return;
	}
}


// Called every frame
void USpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TimeUntilNextSpawn <= 0.f)
	{
		int RandomIndexX = FMath::RandRange(0, MovementGrid->GetGridWidth() - 1);
		int RandomIndexY = FMath::RandRange(0, MovementGrid->GetGridHeight() - 1);
		SpawnItem(ItemToSpawn, FIntVector2(RandomIndexX, RandomIndexY));
		TimeUntilNextSpawn = 1.f;
	}
	TimeUntilNextSpawn -= DeltaTime;


}

void USpawnerComponent::SpawnItem(TSubclassOf<AItem> Item, FIntVector2 SpawnGridCoordinates)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("USpawnerComponent::SpawnItem: Invalid World"));
		return;
	}
	if (!IsValid(Item))
	{
		UE_LOG(LogTemp, Error, TEXT("USpawnerComponent::SpawnItem: Invalid ItemToSpawn"));
		return;
	} 

	if (!IsValid(MovementGrid))
	{
		UE_LOG(LogTemp, Error, TEXT("USpawnerComponent::SpawnItem: Invalid MovementGrid"));
		return;
	}

	FVector SpawnLocation = MovementGrid->GetWorldLocationFromGridIndex(SpawnGridCoordinates.X, SpawnGridCoordinates.Y) + FVector(SpawnDistance, 0.f, 0.f);

	AItem* SpawnedItem = World->SpawnActor<AItem>(Item, SpawnLocation, ParentActor->GetActorRotation());

	if (!SpawnedItem)
	{
		UE_LOG(LogTemp, Error, TEXT("USpawnerComponent::SpawnItem: Couldn't spawn item"));
		return;
	}

	SpawnedItem->SetMovementVelocity(FVector(-1.f, 0.f, 0.f), 5.f);
}

