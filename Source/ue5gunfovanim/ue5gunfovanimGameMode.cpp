// Copyright Epic Games, Inc. All Rights Reserved.

#include "ue5gunfovanimGameMode.h"
#include "ue5gunfovanimCharacter.h"
#include "UObject/ConstructorHelpers.h"

Aue5gunfovanimGameMode::Aue5gunfovanimGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
