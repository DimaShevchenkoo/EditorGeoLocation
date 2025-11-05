// Made by ArmlessMaster

#include "EditorGeoLocationCommands.h"

#define LOCTEXT_NAMESPACE "FEditorGeoLocationModule"

void FEditorGeoLocationCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EditorGeoLocation", "Bring up EditorGeoLocation window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
