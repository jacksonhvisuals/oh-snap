// Jackson Hayes 2026 All Rights Reserved.

#pragma once
#include "Templates/SharedPointer.h"

class FOhSnapStyle
{
public:
	static void Initialize();

	static void Shutdown();

	static TSharedPtr< class ISlateStyle > Get();

	static FName GetStyleSetName();
private:
	static FString InContent(const FString& RelativePath, const ANSICHAR* Extension);
private:
	static TSharedPtr< class FSlateStyleSet > StyleSet;
};
