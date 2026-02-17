// Jackson Hayes 2025 All Rights Reserved.

#include "OhSnapCallbacks.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IMainFrameModule.h"

void FOhSnapCallbacks::SnapActorToActor(bool bTranslation, bool bRotation, bool bReverse)
{
	if (FSlateApplication::Get().GetModifierKeys().IsAltDown())
	{
		UE_LOGFMT(LogTemp, Log, "Alt was held down!");
		// TODO: "inline" component selection UI
		
		/*   |  Snapping options (Oh Snap!)
		 *   |  
		 *   |  Translation
		 *   |  [ X ][ Y ][ Z ]
		 *   |  Rotation
		 *   |  [ X ][ Y ][ Z ]
		 *   |  Scale
		 *   |  [ X ][ Y ][ Z ]
		 *   |
		 *   |  [ ] Make default
		 *   |  
		 *   |          [ Cancel ][ Confirm ]
		 */  
		
		
		// - in UI, read from EditorPreferences, populate checkboxes
		// - on modification, add checkbox for whether this is temporary just for current operation, or should save
		// - on "Confirm Pressed" - dismiss UI, save to EditorConfig, and run operation with provided settings.
		
	}

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

