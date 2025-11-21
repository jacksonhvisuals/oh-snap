// Jackson Hayes 2025 All Rights Reserved.

#pragma once

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

	TSharedPtr<FUICommandInfo> SnapAToB;
	TSharedPtr<FUICommandInfo> SnapBToA;
};