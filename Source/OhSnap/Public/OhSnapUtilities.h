#pragma once
#include "OhSnapTypes.h"

class OhSnapUtils
{
	static FSnapTransformOptions LoadSettings();
	static void SaveSettings(FSnapTransformOptions NewOptions);
	
	static TSharedRef<SWindow> PresentPopup(TSharedRef<SWidget> InWidgetContent);
	
	/** Returns whether the user pressed Confirm or Cancel */
	static bool GetTransformOptionsFromUser(FSnapTransformOptions& OutOptions);
};
