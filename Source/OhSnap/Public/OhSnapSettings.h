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
	
	UPROPERTY(Config, EditAnywhere, Category="Oh Snap", meta=(DisplayName="Snapping Transform Options"))
	FSnapTransformOptions SnapTransformOptions;
};
