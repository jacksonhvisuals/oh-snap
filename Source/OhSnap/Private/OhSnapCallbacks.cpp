// Jackson Hayes 2025 All Rights Reserved.

#include "OhSnapCallbacks.h"

#include "OhSnapUtilities.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IMainFrameModule.h"

void FOhSnapCallbacks::SnapActorToActor(FSnapTransformOptions Options)
{
	if (FSlateApplication::Get().GetModifierKeys().IsAltDown())
	{
		UE_LOGFMT(LogTemp, Log, "Alt was held down!");
		if (!OhSnapUtils::GetTransformOptionsFromUser(Options))
		{
			return;
			
		}
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
	
	/*
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
	*/
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

