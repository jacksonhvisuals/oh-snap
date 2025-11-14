// Copyright Epic Games, Inc. All Rights Reserved.

#include "OhSnap.h"

#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Subsystems/EditorActorSubsystem.h"

#define LOCTEXT_NAMESPACE "FOhSnapModule"
TSharedPtr<FUICommandList> CommandList;
static const FName OhSnapName("OhSnapMenu");

void FOhSnapCommands::RegisterCommands()
{
	UI_COMMAND( SnapAToBCommand, "Snap A to B", "Snap the first selected actor to the second", EUserInterfaceActionType::Button, FInputChord() );
	UI_COMMAND( SnapBToACommand, "Snap B to A", "Snap the second selected actor to the first", EUserInterfaceActionType::Button, FInputChord() );
}

const FOhSnapCommands& FOhSnapCommands::Get()
{
	return TCommands<FOhSnapCommands>::Get();
}

void FOhSnapModule::StartupModule()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	FOhSnapCommands::Register();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FOhSnapModule::RegisterSnapButtons));
}

void FOhSnapModule::ShutdownModule()
{
    UnregisterSnapButtons();
	FOhSnapCommands::Unregister();
}

void FOhSnapModule::RegisterSnapButtons()
{
	UToolMenus* ToolMenus = UToolMenus::Get();
	UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.LevelEditorSceneOutliner.ContextMenu.SnapAlignSubMenu");
	if (!Menu)
	{
		UE_LOG(LogTemp, Log, TEXT("OhSnap could not find LevelEditor.ActorContextMenu.SnapAlignSubMenu"));
		return;
	}
	FToolMenuSection& Section = Menu->FindOrAddSection("SnapAlign");
	Section.InsertPosition = FToolMenuInsert("SnapOriginToGrid", EToolMenuInsertType::Before);
	FToolMenuEntry& Entry = Section.AddDynamicEntry("OhSnapEntry", FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
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
		
		AActor* FirstActor = SelectedActors[0];
		AActor* SecondActor = SelectedActors[1];
		
		FToolUIActionChoice SnapAToBChoice(FExecuteAction::CreateLambda([FirstActorWeak = TWeakObjectPtr<AActor>(FirstActor), SecondActorWeak = TWeakObjectPtr<AActor>(SecondActor)]()
		{
			TStrongObjectPtr<AActor> FirstActorStrong = FirstActorWeak.Pin();
			TStrongObjectPtr<AActor> SecondActorStrong = SecondActorWeak.Pin();

			// TODO: Could I just use the boolean operator instead of .IsValid()?
			if (FirstActorStrong.IsValid() && SecondActorStrong.IsValid())
			{
				FTransform TransformA = FirstActorStrong->GetTransform();
				// TODO: Support Rot / Scale validation
				SecondActorStrong->SetActorTransform(TransformA);
			}
		}
			));
		FToolUIActionChoice SnapBToAChoice(FExecuteAction::CreateLambda([FirstActorWeak = TWeakObjectPtr<AActor>(FirstActor), SecondActorWeak = TWeakObjectPtr<AActor>(SecondActor)]()
		{
			TStrongObjectPtr<AActor> FirstActorStrong = FirstActorWeak.Pin();
			TStrongObjectPtr<AActor> SecondActorStrong = SecondActorWeak.Pin();
			FOhSnapCommands::Get().SnapAToBCommand.

			// TODO: Could I just use the boolean operator instead of .IsValid()?
			if (FirstActorStrong.IsValid() && SecondActorStrong.IsValid())
			{
				FTransform TransformB = FirstActorStrong->GetTransform();
				// TODO: Support Rot / Scale validation
				FirstActorStrong->SetActorTransform(TransformB);
			}
		}));

		FString FirstActorLabel = FirstActor->GetActorLabel();
		FString SecondActorLabel = SecondActor->GetActorLabel();

		FString SnapAToBLabel = "Snap " + FirstActorLabel + " -> " + SecondActorLabel;
		FString SnapBToALabel = "Snap " + SecondActorLabel + " -> " + FirstActorLabel;
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
