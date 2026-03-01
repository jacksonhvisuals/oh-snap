// Jackson Hayes 2025 All Rights Reserved.

#include "OhSnap.h"

#include "LevelEditor.h"
#include "OhSnapCallbacks.h"
#include "OhSnapCommands.h"
#include "OhSnapSettings.h"
#include "OhSnapUtilities.h"
#include "ToolMenus.h"
#include "Engine/DeveloperSettings.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Widgets/OhSnapStyle.h"
#include "Widgets/SnapTransformSettingCustomization.h"

#define LOCTEXT_NAMESPACE "FOhSnapModule"
static const FName OhSnapName("OhSnapMenu");
static const FName OhSnapSettingsName("OhSnapSettings");

void FOhSnapModule::StartupModule()
{
	FOhSnapStyle::Initialize();
	FOhSnapCommands::Register();
	RegisterGlobalOhSnapCommands();

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FOhSnapModule::RegisterSnapButtons));
	
	static const FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor); 
	PropertyModule.RegisterCustomClassLayout(OhSnapSettingsName, FOnGetDetailCustomizationInstance::CreateStatic(&FSnapTransformSettingCustomization::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FOhSnapModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(OhSnapSettingsName);
	}
	
	FOhSnapStyle::Shutdown();
    UnregisterSnapButtons();
	FOhSnapCommands::Unregister();
}

void FOhSnapModule::RegisterGlobalOhSnapCommands()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedRef<FUICommandList> LECommandList = LevelEditorModule.GetGlobalLevelEditorActions();
    FUICommandList& ActionList = *LECommandList;

    const FOhSnapCommands& Commands = FOhSnapCommands::Get();
	
    ActionList.MapAction( Commands.SnapAToB, FExecuteAction::CreateLambda([] ()
    {
    	FSnapTransformOptions Options = OhSnapUtils::LoadSettings();
		FOhSnapCallbacks::SnapActorToActor(Options, false);
    }), FCanExecuteAction::CreateStatic( &FOhSnapCallbacks::SnapActorToActor_CanExecute) );

    ActionList.MapAction( Commands.SnapBToA, FExecuteAction::CreateStatic([] ()
    {
    	FSnapTransformOptions Options = OhSnapUtils::LoadSettings();
		FOhSnapCallbacks::SnapActorToActor(Options, true);
    }), FCanExecuteAction::CreateStatic( &FOhSnapCallbacks::SnapActorToActor_CanExecute) );
}

void FOhSnapModule::RegisterSnapButtons()
{
	UToolMenus* ToolMenus = UToolMenus::Get();
	UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.LevelEditorSceneOutliner.ContextMenu.SnapAlignSubMenu");
	if (!Menu)
	{
		return;
	}
	FToolMenuSection& Section = Menu->FindOrAddSection("SnapAlign");
	Section.InsertPosition = FToolMenuInsert("SnapOriginToGrid", EToolMenuInsertType::Before);
	FToolMenuEntry& Entry = Section.AddDynamicEntry("OhSnapEntry", FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
	{
		FToolUIActionChoice SnapAToBChoice(FExecuteAction::CreateLambda([] ()
		{
			FSnapTransformOptions Options = OhSnapUtils::LoadSettings();
			FOhSnapCallbacks::SnapActorToActor(Options, false);
		}));

		FToolUIActionChoice SnapBToAChoice(FExecuteAction::CreateLambda([] ()
		{
			FSnapTransformOptions Options = OhSnapUtils::LoadSettings();
			FOhSnapCallbacks::SnapActorToActor(Options, true);
		}));

		UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
		if (!EditorActorSubsystem)
		{
			return;
		}

		TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();

		if (SelectedActors.Num() != 2)
		{
			return;
		}
		
		FString FirstActorLabel = SelectedActors[0]->GetActorLabel();
		FString SecondActorLabel = SelectedActors[1]->GetActorLabel();
		FString SnapAToBLabel = "Snap to " + SecondActorLabel;
		FString SnapBToALabel = "Snap to " + FirstActorLabel;
		
		InSection.AddEntry(FToolMenuEntry::InitMenuEntry(FName("Snap A to B"), FText::FromString(SnapAToBLabel), FText::FromString("Snaps the first actor to the second actor's transform"), FSlateIcon(), SnapAToBChoice));
		InSection.AddEntry(FToolMenuEntry::InitMenuEntry(FName("Snap B to A"), FText::FromString(SnapBToALabel), FText::FromString("Snaps the second actor to the first actor's transform"), FSlateIcon(), SnapBToAChoice));
	}));
}

void FOhSnapModule::UnregisterSnapButtons()
{
	if (UToolMenus* Menus = UToolMenus::Get())
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		TSharedRef<FUICommandList> LECommandList = LevelEditorModule.GetGlobalLevelEditorActions();
		LECommandList->UnmapAction(FOhSnapCommands::Get().SnapAToB);
		LECommandList->UnmapAction(FOhSnapCommands::Get().SnapBToA);

		if (Menus->IsMenuRegistered(OhSnapName))
		{
			UToolMenus::UnregisterOwner(this);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOhSnapModule, OhSnap)
