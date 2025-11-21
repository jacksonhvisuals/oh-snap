// Jackson Hayes 2025 All Rights Reserved.

#include "OhSnapCommands.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FOhSnapModule"

void FOhSnapCommands::RegisterCommands()
{
	UI_COMMAND( SnapAToB, "Snap A to B", "Snap the first selected actor to the second", EUserInterfaceActionType::Button, FInputChord() );
	UI_COMMAND( SnapBToA, "Snap B to A", "Snap the second selected actor to the first", EUserInterfaceActionType::Button, FInputChord() );
}

const FOhSnapCommands& FOhSnapCommands::Get()
{
	return TCommands<FOhSnapCommands>::Get();
}
#undef LOCTEXT_NAMESPACE
