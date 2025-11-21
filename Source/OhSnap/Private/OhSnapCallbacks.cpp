
#include "OhSnapCallbacks.h"
// Jackson Hayes 2025 All Rights Reserved.

#include "Subsystems/EditorActorSubsystem.h"

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

