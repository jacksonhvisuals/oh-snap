// Copyright Epic Games, Inc. All Rights Reserved.

#include "OhSnap.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Engine/DeveloperSettings.h"
#include "Subsystems/EditorActorSubsystem.h"

#define LOCTEXT_NAMESPACE "FOhSnapModule"
TSharedPtr<FUICommandList> CommandList;
static const FName OhSnapName("OhSnapMenu");

UOhSnapSettings::UOhSnapSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CategoryName = FName(TEXT("LevelEditor"));
}

void FOhSnapCallbacks::SnapActorToActor(bool bTranslation, bool bRotation, bool bReverse)
{
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
		
	AActor* TargetActor;
	FTransform DesiredTransform;
	
	if (bReverse)
	{
		DesiredTransform = SelectedActors[0]->GetTransform();
		TargetActor = SelectedActors[1];
	}
	else
	{
		DesiredTransform = SelectedActors[1]->GetTransform();
		TargetActor = SelectedActors[0];
	}
	
	{
		const FScopedTransaction Transaction(NSLOCTEXT("OhSnap", "LevelEditorSnapActorToActor", "Snap Actor to Actor"));
		
		TargetActor->SetFlags(RF_Transactional);
		TargetActor->Modify();
		if (USceneComponent* RootComp = TargetActor->GetRootComponent())
		{
			RootComp->SetFlags(RF_Transactional);
			RootComp->Modify();
		}
		
		if (bTranslation)
		{
			TargetActor->SetActorLocation(DesiredTransform.GetLocation());
		}
	
		if (bRotation)
		{
			TargetActor->SetActorRotation(DesiredTransform.GetRotation());
		}
	}
}

bool FOhSnapCallbacks::SnapActorToActor_CanExecute()
{
	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		return false;
	}

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();

	if (SelectedActors.Num() != 2)
	{
		return false;
	}
	
	return true;
}

void FOhSnapCommands::RegisterCommands()
{
	UI_COMMAND( SnapAToB, "Snap A to B", "Snap the first selected actor to the second", EUserInterfaceActionType::Button, FInputChord() );
	UI_COMMAND( SnapBToA, "Snap B to A", "Snap the second selected actor to the first", EUserInterfaceActionType::Button, FInputChord() );
}

const FOhSnapCommands& FOhSnapCommands::Get()
{
	return TCommands<FOhSnapCommands>::Get();
}

void FOhSnapModule::StartupModule()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	FOhSnapCommands::Register();
	BindGlobalOhSnapCommands();
	LevelEditorModule.GetGlobalLevelEditorActions()->Append(CommandList.ToSharedRef());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FOhSnapModule::RegisterSnapButtons));
}

void FOhSnapModule::ShutdownModule()
{
    UnregisterSnapButtons();
	FOhSnapCommands::Unregister();
}

void FOhSnapModule::BindGlobalOhSnapCommands()
{
    check( !CommandList.IsValid() );

    CommandList = MakeShareable( new FUICommandList );

    const FOhSnapCommands& Commands = FOhSnapCommands::Get();
    FUICommandList& ActionList = *CommandList;

    ActionList.MapAction( Commands.SnapAToB, FExecuteAction::CreateLambda([] ()
    {
    	UOhSnapSettings* Settings = GetMutableDefault<UOhSnapSettings>();
		FOhSnapCallbacks::SnapActorToActor(Settings->bIncludeTranslation, Settings->bIncludeRotation, false);
    }), FCanExecuteAction::CreateStatic( &FOhSnapCallbacks::SnapActorToActor_CanExecute) );
	
    ActionList.MapAction( Commands.SnapBToA, FExecuteAction::CreateStatic([] ()
    {
    	UOhSnapSettings* Settings = GetMutableDefault<UOhSnapSettings>();
		FOhSnapCallbacks::SnapActorToActor(Settings->bIncludeTranslation, Settings->bIncludeRotation, true);
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
			UOhSnapSettings* Settings = GetMutableDefault<UOhSnapSettings>();
			FOhSnapCallbacks::SnapActorToActor(Settings->bIncludeTranslation, Settings->bIncludeRotation, false);
		}));
		
		FToolUIActionChoice SnapBToAChoice(FExecuteAction::CreateLambda([] ()
		{
			UOhSnapSettings* Settings = GetMutableDefault<UOhSnapSettings>();
			FOhSnapCallbacks::SnapActorToActor(Settings->bIncludeTranslation, Settings->bIncludeRotation, true);
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
		if (Menus->IsMenuRegistered(OhSnapName))
		{
			UToolMenus::UnregisterOwner(this);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOhSnapModule, OhSnap)
