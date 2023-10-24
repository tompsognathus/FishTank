// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class FISHGAME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMovementVelocity(FVector MovementDirection, float MovementSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int Value = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool DoesRotate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float RotationSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* MeshComponent;

private:
	void Rotate(float DeltaTime);
	void Move(float DeltaTime);

private:
	FVector MovementVelocity = FVector::Zero();
	
};
