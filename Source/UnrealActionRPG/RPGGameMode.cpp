
#include "RPGGameMode.h"
#include "RPGCharacter.h"

ARPGGameMode::ARPGGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	DefaultPawnClass = ARPGCharacter::StaticClass();
}