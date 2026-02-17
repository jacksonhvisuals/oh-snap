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
	
	/** Include Translation when snapping Actors to each other */
	UPROPERTY(Config, EditAnywhere, Category="Actor to Actor", meta=(DisplayName="Include Translation"))
	bool bIncludeTranslation = true;
	
	/** Include Rotation when snapping Actors to each other */
	UPROPERTY(Config, EditAnywhere, Category="Actor to Actor", meta=(DisplayName="Include Rotation"))
	bool bIncludeRotation = true;
	
	/** Include Scale when snapping Actors to each other */
	UPROPERTY(Config, EditAnywhere, Category="Actor to Actor", meta=(DisplayName="Include Scale"))
	bool bIncludeScale = false;
	
	UPROPERTY(Config, EditAnywhere, Category="Actor to Actor")
	FSnapTransformOptions SnapTransformOptions;
	
	/* It looks like we need to register our custom property type layout with the FPropertyEditorModule
	 * Specifically, see how FImportantToggleSettingCustomization implements IDetailCustomization.
	 * 
	 * FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor); 
	 * PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
	 */
};
