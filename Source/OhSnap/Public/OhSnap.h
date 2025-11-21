// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FOhSnapModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    
    void RegisterSnapButtons();
    void UnregisterSnapButtons();
    
    void BindGlobalOhSnapCommands();
private:
    TSharedPtr<FUICommandList> CommandList;
};
