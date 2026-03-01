#include "OhSnapUtilities.h"

#include "OhSnapLog.h"
#include "OhSnapSettings.h"
#include "Interfaces/IMainFrameModule.h"
#include "Widgets/SSnapTransformPrefs.h"
#include "OhSnapTypes.h"
#include "Dialog/SCustomDialog.h"

class SCustomDialog;

#define LOCTEXT_NAMESPACE "OhSnap"

FSnapTransformOptions OhSnapUtils::LoadSettings()
{
	UOhSnapSettings* Settings = GetMutableDefault<UOhSnapSettings>();
	if (!IsValid(Settings))
	{
		UE_LOGFMT(LogOhSnap, Error, "Failed to load **Oh Snap** settings");
		return FSnapTransformOptions();
	}
	return Settings->SnapTransformOptions;
}

void OhSnapUtils::SaveSettings(FSnapTransformOptions NewOptions)
{
	UOhSnapSettings* Settings = GetMutableDefault<UOhSnapSettings>();
	if (!IsValid(Settings))
	{
		UE_LOGFMT(LogOhSnap, Error, "Failed to save **Oh Snap** settings");
		return;
	}
	Settings->SnapTransformOptions = NewOptions;
	Settings->SaveConfig();
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
			.HAlign(HAlign_Fill)
			.AutoHeight()
			[
				SNew(SSnapTransformPreferences)
				.TransformOptions(CurrentOptions)
				.OnOptionsChanged_Lambda([&CurrentOptions] (const FSnapTransformOptions& InOptions)
				{
					CurrentOptions = InOptions;
				})
				
			]
			+ SVerticalBox::Slot()
			.Padding(0, 8)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoWidth()
				[
					SNew(SCheckBox)
					.IsChecked({false})
					.ToolTipText(LOCTEXT("MakeDefaultToolTip", "Save this configuration to Oh Snap settings"))
					.OnCheckStateChanged_Lambda([&bSaveAsDefaults] (ECheckBoxState InState)
					{
						bSaveAsDefaults = InState == ECheckBoxState::Checked;
					})
					.Content()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.Padding(2.0f, 0.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
								.Text(LOCTEXT("MakeDefaultText", "Make default"))
						]
					]
				]
			]
			
			
		]
		.Buttons({
			SCustomDialog::FButton(LOCTEXT("ConfirmSnappingOptions", "Confirm"), FSimpleDelegate(), SCustomDialog::EButtonRole::Confirm),
			SCustomDialog::FButton(LOCTEXT("Cancel", "Cancel"), FSimpleDelegate(), SCustomDialog::EButtonRole::Cancel),
	});

	// Show window and get result. Result < 0 = closed, 0 = Confirm, 1 = Cancel
	const int32 Result = SnappingOptionsDialog->ShowModal();
	if (Result != 0)
	{
		return false;
	}
	
	OutOptions = CurrentOptions;
	
	if (bSaveAsDefaults)
	{
		SaveSettings(OutOptions);
	}
	
	return true;
}

#undef LOCTEXT_NAMESPACE