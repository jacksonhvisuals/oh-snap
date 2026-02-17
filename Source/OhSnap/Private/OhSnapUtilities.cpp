#include "OhSnapUtilities.h"

#include "Interfaces/IMainFrameModule.h"
#include "Widgets/SSnapTransformPrefs.h"
#include "OhSnapTypes.h"

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
	
	// create a new slate widget that contains SSnapTransformPrefs
	// + the checkbox & the confirm / cancel buttons
	
	// if bMakeDefault is true, persist settings 
	// OutOptions = Options;
	// return true/false
	return false;
}
