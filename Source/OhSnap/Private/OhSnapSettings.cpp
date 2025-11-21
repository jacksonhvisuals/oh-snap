#include "OhSnapSettings.h"

UOhSnapSettings::UOhSnapSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CategoryName = FName(TEXT("LevelEditor"));
}
