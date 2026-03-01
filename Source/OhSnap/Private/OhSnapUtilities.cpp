// Copyright Jackson Hayes. All Rights Reserved.

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

bool OhSnapUtils::GetTransformOptionsFromUser(FSnapTransformOptions& OutOptions, const FString& TargetActorName, const FString& SourceActorName)
{
	FSnapTransformOptions CurrentOptions = LoadSettings();
	bool bSaveAsDefaults = false;

	const bool bShowActorNames = !TargetActorName.IsEmpty() && !SourceActorName.IsEmpty();

	TSharedRef<SCustomDialog> SnappingOptionsDialog = SNew(SCustomDialog)
		.Title(FText(LOCTEXT("OhSnap_GetSnappingOptions", "Snapping options")))
		.UseScrollBox(false)
		.HAlignContent(HAlign_Fill)
		.RootPadding(FMargin(8))
		.ButtonAreaPadding(FMargin({20, 16, 4, 4}))
		.Content()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				// Zero-height slot to enforce a minimum dialog width
				SNew(SBox)
				.MinDesiredWidth(340.f)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 8)
			.HAlign(HAlign_Left)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
				.Padding(FMargin(8, 6))
				.Visibility(bShowActorNames ? EVisibility::Visible : EVisibility::Collapsed)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TargetActorName))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
						.OverflowPolicy(ETextOverflowPolicy::Ellipsis)
						.ColorAndOpacity(FSlateColor::UseForeground())
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(10, 0)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("\u2192")))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
						.ColorAndOpacity(FSlateColor::UseSubduedForeground())
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString(SourceActorName))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
						.OverflowPolicy(ETextOverflowPolicy::Ellipsis)
						.ColorAndOpacity(FSlateColor::UseForeground())
					]
				]
			]
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
			.AutoHeight()
			.Padding(0, 8)
			[
				SNew(SSeparator)
				.Orientation(Orient_Horizontal)
			]
			+ SVerticalBox::Slot()
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
			SCustomDialog::FButton(LOCTEXT("Cancel", "Cancel"), FSimpleDelegate(), SCustomDialog::EButtonRole::Cancel),
			SCustomDialog::FButton(LOCTEXT("ConfirmSnappingOptions", "Confirm"), FSimpleDelegate(), SCustomDialog::EButtonRole::Confirm).SetPrimary(true),
	});

	// Show window and get result. Result < 0 = closed, 0 = Cancel, 1 = Confirm
	const int32 Result = SnappingOptionsDialog->ShowModal();
	if (Result != 1)
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