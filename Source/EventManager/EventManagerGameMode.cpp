// // Copyright Pablo Rodrigo Sanchez, Inc. All Rights Reserved.

#include "EventManagerGameMode.h"
#include "UObject/ConstructorHelpers.h"

AEventManagerGameMode::AEventManagerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}