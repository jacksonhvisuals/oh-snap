// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Framework/Commands/Commands.h"

class FOhSnapCommands : public TCommands<FOhSnapCommands>
{
public:
	
	virtual void RegisterCommands() override;

	OHSNAP_API FORCENOINLINE static const FOhSnapCommands& Get();
	
	FOhSnapCommands()
		: TCommands<FOhSnapCommands>(
			TEXT("OhSnap"),
			NSLOCTEXT("Contexts", "OhSnap", "Oh Snap"),
			NAME_None,
			FAppStyle::GetAppStyleSetName())
	{}

	TSharedPtr<FUICommandInfo> SnapAToBCommand;
	
	TSharedPtr<FUICommandInfo> SnapBToACommand;
};

class FOhSnapModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    
    void RegisterSnapButtons();
    void UnregisterSnapButtons();
    
};
