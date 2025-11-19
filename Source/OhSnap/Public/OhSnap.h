// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "Modules/ModuleManager.h"
#include "Framework/Commands/Commands.h"
#include "OhSnap.generated.h"

class OHSNAP_API FOhSnapCallbacks
{
public:
	static void SnapActorToActor(bool bTranslation = true, bool bRotation = true, bool bReverse = false);
	static bool SnapActorToActor_CanExecute();
};

/**
 * Configure snapping settings within the Level Outliner
 */
UCLASS(config=EditorPerProjectUserSettings, meta=( DisplayName="Snapping" ), MinimalAPI)
class UOhSnapSettings
	: public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UOhSnapSettings(const FObjectInitializer& ObjectInitializer);
	
	/** Include Translation when snapping Actors to each other */
	UPROPERTY(Config, EditAnywhere, Category="Actors", meta=(DisplayName="Include Translation"))
	bool bIncludeTranslation = true;
	
	/** Include Rotation when snapping Actors to each other */
	UPROPERTY(Config, EditAnywhere, Category="Actors", meta=(DisplayName="Include Rotation"))
	bool bIncludeRotation = true;
};

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
