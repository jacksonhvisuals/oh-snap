#include "OhSnapUtilities.h"

#include "Interfaces/IMainFrameModule.h"
#include "Widgets/SSnapTransformPrefs.h"
#include "OhSnapTypes.h"
#include "Dialog/SCustomDialog.h"

class SCustomDialog;

#define LOCTEXT_NAMESPACE "OhSnap"

FSnapTransformOptions OhSnapUtils::LoadSettings()
{
	return FSnapTransformOptions();
}

void OhSnapUtils::SaveSettings(FSnapTransformOptions NewOptions)
{
}

TSharedRef<SWindow> OhSnapUtils::PresentPopup(TSharedRef<SWidget> InWidgetContent)
{
	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(FText::FromString(TEXT("Snapping options")))
		.ClientSize(FVector2D(400, 300))
		[
			InWidgetContent
		];
		
	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	TSharedPtr<SWindow> Parent = MainFrame.GetParentWindow();
	if (Parent.IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(Window, Parent.ToSharedRef());
	}
	return Window;
}

bool OhSnapUtils::GetTransformOptionsFromUser(FSnapTransformOptions& OutOptions)
{
	FSnapTransformOptions CurrentOptions = LoadSettings();
	bool bSaveAsDefaults = false;
	
	TSharedRef<SCustomDialog> SnappingOptionsDialog = SNew(SCustomDialog)
		.Title(FText(LOCTEXT("OhSnap_GetSnappingOptions", "Snapping options")))
		.Content()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SSnapTransformPreferences)
				.TransformOptions(CurrentOptions)
				.OnOptionsChanged_Lambda([&CurrentOptions] (const FSnapTransformOptions& InOptions)
				{
					CurrentOptions = InOptions;
				})
				
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.IsChecked({false})
				.OnCheckStateChanged_Lambda([&bSaveAsDefaults] (ECheckBoxState InState)
				{
					bSaveAsDefaults = InState == ECheckBoxState::Checked;
				})
			]
			
			
		]
		.Buttons({
			SCustomDialog::FButton(LOCTEXT("ConfirmSnappingOptions", "Confirm")),
			SCustomDialog::FButton(LOCTEXT("Cancel", "Cancel"))
	});

	// show window and get result
	const int32 Result = SnappingOptionsDialog->ShowModal();
	const bool bWindowClosed = Result < 0;
	const bool bSkipped = Result == 1;
	
	if (bWindowClosed)
	{
		return false; // window closed
	}

	if (bSkipped)
	{
		return false; // user hit cancel
	}
	
	OutOptions = CurrentOptions;
	
	if (bSaveAsDefaults)
	{
		SaveSettings(OutOptions);
	}
	
	return true;
}

#undef LOCTEXT_NAMESPACE