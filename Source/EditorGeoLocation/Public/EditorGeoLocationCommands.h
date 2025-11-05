// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "EditorGeoLocationStyle.h"

class FEditorGeoLocationCommands : public TCommands<FEditorGeoLocationCommands>
{
public:

	FEditorGeoLocationCommands()
		: TCommands<FEditorGeoLocationCommands>(TEXT("EditorGeoLocation"), NSLOCTEXT("Contexts", "EditorGeoLocation", "EditorGeoLocation Plugin"), NAME_None, FEditorGeoLocationStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};