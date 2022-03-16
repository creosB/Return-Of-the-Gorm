// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReturnoftheGormGameMode.h"
#include "ContraCharacter.h"
#include "UObject/ConstructorHelpers.h"

AReturnoftheGormGameMode::AReturnoftheGormGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Pawn/BP_Contra"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
void AReturnoftheGormGameMode::PawnKilled(APawn* PawnKiled)
{

}