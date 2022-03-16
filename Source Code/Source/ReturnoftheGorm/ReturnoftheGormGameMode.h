// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReturnoftheGormGameMode.generated.h"

UCLASS(minimalapi)
class AReturnoftheGormGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AReturnoftheGormGameMode();

	virtual void PawnKilled(APawn* PawnKiled);
};



