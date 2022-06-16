// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheBackRoomGameMode.h"
#include "TheBackRoomCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheBackRoomGameMode::ATheBackRoomGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
