// Copyright Epic Games, Inc. All Rights Reserved.

#include "PSoul/Public/GameFramework/PSoulGameMode.h"
#include "UObject/ConstructorHelpers.h"

APSoulGameMode::APSoulGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
