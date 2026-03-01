// Jackson Hayes 2025 All Rights Reserved.

#include "OhSnapCallbacks.h"

#include "OhSnapUtilities.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IMainFrameModule.h"

void FOhSnapCallbacks::SnapActorToActor(FSnapTransformOptions Options, bool bReverse)
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

	AActor* SourceActor;
	AActor* TargetActor;

	if (bReverse)
	{
		SourceActor = SelectedActors[0];
		TargetActor = SelectedActors[1];
	}
	else
	{
		SourceActor = SelectedActors[1];
		TargetActor = SelectedActors[0];
	}

	if (FSlateApplication::Get().GetModifierKeys().IsAltDown())
	{
		if (!OhSnapUtils::GetTransformOptionsFromUser(Options, TargetActor->GetActorLabel(), SourceActor->GetActorLabel()))
		{
			return;
		}
	}

	const FTransform SourceTransform = SourceActor->GetTransform();

	const FScopedTransaction Transaction(NSLOCTEXT("OhSnap", "LevelEditorSnapActorToActor", "Snap Actor to Actor"));

	TargetActor->SetFlags(RF_Transactional);
	TargetActor->Modify();
	if (USceneComponent* RootComp = TargetActor->GetRootComponent())
	{
		RootComp->SetFlags(RF_Transactional);
		RootComp->Modify();
	}

	if (Options.Translation.bEnabled)
	{
		const FVector CurrentLocation = TargetActor->GetActorLocation();
		const FVector SourceLocation = SourceTransform.GetLocation();
		FVector NewLocation;
		NewLocation.X = Options.Translation.X ? SourceLocation.X : CurrentLocation.X;
		NewLocation.Y = Options.Translation.Y ? SourceLocation.Y : CurrentLocation.Y;
		NewLocation.Z = Options.Translation.Z ? SourceLocation.Z : CurrentLocation.Z;
		TargetActor->SetActorLocation(NewLocation);
	}

	if (Options.Rotation.bEnabled)
	{
		const FRotator CurrentRotation = TargetActor->GetActorRotation();
		const FRotator SourceRotation = SourceTransform.GetRotation().Rotator();
		FRotator NewRotation;
		NewRotation.Roll  = Options.Rotation.X ? SourceRotation.Roll  : CurrentRotation.Roll;
		NewRotation.Pitch = Options.Rotation.Y ? SourceRotation.Pitch : CurrentRotation.Pitch;
		NewRotation.Yaw   = Options.Rotation.Z ? SourceRotation.Yaw   : CurrentRotation.Yaw;
		TargetActor->SetActorRotation(NewRotation);
	}

	if (Options.Scale.bEnabled)
	{
		const FVector CurrentScale = TargetActor->GetActorScale3D();
		const FVector SourceScale = SourceTransform.GetScale3D();
		FVector NewScale;
		NewScale.X = Options.Scale.X ? SourceScale.X : CurrentScale.X;
		NewScale.Y = Options.Scale.Y ? SourceScale.Y : CurrentScale.Y;
		NewScale.Z = Options.Scale.Z ? SourceScale.Z : CurrentScale.Z;
		TargetActor->SetActorScale3D(NewScale);
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
