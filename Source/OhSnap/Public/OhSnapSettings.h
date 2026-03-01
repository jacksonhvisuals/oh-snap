// Jackson Hayes 2025 All Rights Reserved.

#pragma once

#include "OhSnapTypes.h"
#include "UObject/Object.h"
#include "Engine/DeveloperSettings.h"
#include "OhSnapSettings.generated.h"

/**
* Configure Actor snapping settings within the Level Outliner
 */
UCLASS(config=EditorPerProjectUserSettings, meta=( DisplayName="Snapping" ), MinimalAPI)
class UOhSnapSettings
	: public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UOhSnapSettings(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Config, EditAnywhere, Category="Actor to Actor", meta=(DisplayName="Alt Opens Inline Dialog",
		ToolTip="When enabled, holding Alt while triggering a snap opens the inline options dialog. Disable this if your snap shortcut already includes the Alt key."))
	bool bShowInlineDialog = true;

	UPROPERTY(Config, EditAnywhere, Category="Actor to Actor", meta=(DisplayName="Snapping Transform Options"))
	FSnapTransformOptions SnapTransformOptions;
};
