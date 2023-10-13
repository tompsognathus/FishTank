// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FishGameCharacter.generated.h"

UCLASS(Blueprintable)
class AFishGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFishGameCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

};

