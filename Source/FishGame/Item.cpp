// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/ArrowComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	UArrowComponent* ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (!IsValid(ArrowComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("AItem::AItem: Invalid ArrowComponent"));
		return;
	}
	this->SetRootComponent(ArrowComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	if (!IsValid(MeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("AItem::AItem: Invalid MeshComponent"));
		return;
	}
	MeshComponent->SetupAttachment(ArrowComponent);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoesRotate)
	{
		Rotate(DeltaTime);
	}
	Move(DeltaTime);

}

void AItem::Rotate(float DeltaTime)
{
	// Rotate around the Z axis
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += RotationSpeed * DeltaTime;
	SetActorRotation(Rotation);
}

void AItem::Move(float DeltaTime)
{
	FVector NewActorLocation = GetActorLocation() + MovementVelocity;
	SetActorLocation(NewActorLocation);
}

void AItem::SetMovementVelocity(FVector MovementDirection, float MovementSpeed)
{
	MovementVelocity = MovementDirection * MovementSpeed;
}

