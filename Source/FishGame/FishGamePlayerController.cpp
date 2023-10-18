// Copyright Epic Games, Inc. All Rights Reserved.

#include "FishGamePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "FishGameCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Kismet/GameplayStatics.h"
#include "MovementGrid.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFishGamePlayerController::AFishGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AFishGamePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Get player pawn
	APawn* PlayerPawn = GetPawn();
	if (!IsValid(PlayerPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("AFishGamePlayerController::BeginPlay: Invalid PlayerPawn"));
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("AFishGamePlayerController::BeginPlay: Invalid World"));
		return;
	}

	// Get Movement Grid
	TArray<AActor*> FoundMovementGridActors;
	UGameplayStatics::GetAllActorsOfClass(World, AMovementGrid::StaticClass(), FoundMovementGridActors);

	if (FoundMovementGridActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("AFishGamePlayerController::BeginPlay: No Movement Grid Found"));
		return;
	}

	MovementGrid = Cast<AMovementGrid>(FoundMovementGridActors[0]);
	if (!IsValid(MovementGrid))
	{
		UE_LOG(LogTemp, Error, TEXT("AFishGamePlayerController::BeginPlay: Invalid Movement Grid"));
		return;
	}

	MovementGridWidth = MovementGrid->GetGridWidth();	
	MovementGridHeight = MovementGrid->GetGridHeight();

	InitialLocation = MovementGrid->GetWorldLocationFromGridIndex(0, 0);
	PlayerPawn->SetActorLocation(InitialLocation);
	CachedDestination = InitialLocation;
}


void AFishGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MoveFishPawn();
}

void AFishGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		//// Setup mouse input events
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AFishGamePlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AFishGamePlayerController::OnSetDestinationTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AFishGamePlayerController::OnSetDestinationReleased);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AFishGamePlayerController::OnSetDestinationReleased);

		//// Setup touch input events
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AFishGamePlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AFishGamePlayerController::OnTouchTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AFishGamePlayerController::OnTouchReleased);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AFishGamePlayerController::OnTouchReleased);

		// Setup keyboard input events
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFishGamePlayerController::OnMove);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFishGamePlayerController::OnInputStarted()
{
	StopMovement();
}

void AFishGamePlayerController::OnMove(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Update fish grid index
	if (MovementVector.X > 0 && CurrentGridIdx.X < MovementGridWidth - 1)
	{
		CurrentGridIdx.X += 1;
	}
	else if (MovementVector.X < 0 && CurrentGridIdx.X > 0)
	{
		CurrentGridIdx.X -= 1;
	}
	else if (MovementVector.Y > 0 && CurrentGridIdx.Y < MovementGridHeight - 1)
	{
		CurrentGridIdx.Y += 1;
	}
	else if (MovementVector.Y < 0 && CurrentGridIdx.Y > 0)
	{
		CurrentGridIdx.Y -= 1;
	}

	CachedDestination = MovementGrid->GetWorldLocationFromGridIndex(CurrentGridIdx.X, CurrentGridIdx.Y);
}

void AFishGamePlayerController::MoveFishPawn()
{
	// Teleport to target location
	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("AFishGamePlayerController::OnMove: Invalid ControlledPawn"));
		return;
	}

	FVector Destination = FMath::Lerp(ControlledPawn->GetActorLocation(), CachedDestination, MoveTime);
	ControlledPawn->SetActorLocation(Destination);

	FVector LookAtTarget = Destination + FVector(1.f, 0.f, 0.f) * 100.f;
	// Rotate towards lookat target


}

// Triggered every frame when the input is held down
void AFishGamePlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AFishGamePlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AFishGamePlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AFishGamePlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
