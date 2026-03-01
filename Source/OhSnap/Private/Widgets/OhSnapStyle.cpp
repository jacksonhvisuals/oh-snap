#include "Widgets/OhSnapStyle.h"
#include "Brushes/SlateBoxBrush.h"
#include "Styling/SlateStyleRegistry.h"
#include "Brushes/SlateImageBrush.h"
#include "Styling/SlateTypes.h"
#include "Misc/Paths.h"
#include "Styling/CoreStyle.h"
#include "Interfaces/IPluginManager.h"
#include "SlateOptMacros.h"
#include "Styling/SlateStyle.h"


#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( FOhSnapStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )
#define IMAGE_PLUGIN_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush( FOhSnapStyle::InContent( RelativePath, ".svg" ), __VA_ARGS__ )
#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

FString FOhSnapStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("OhSnap"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr< FSlateStyleSet > FOhSnapStyle::StyleSet = nullptr;
TSharedPtr< class ISlateStyle > FOhSnapStyle::Get() { return StyleSet; }

FName FOhSnapStyle::GetStyleSetName()
{
	static FName OhSnapStyleName(TEXT("OhSnapStyle"));
	return OhSnapStyleName;
}

// BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void FOhSnapStyle::Initialize()
{
	// Const icon sizes
	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("OhSnap"));
	if (!Plugin)
	{
		return;
	}

	// const FString PluginContentDir = Plugin->GetContentDir();
	// StyleSet->SetContentRoot(PluginContentDir);
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));
	const FTextBlockStyle& NormalText = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

	{ // Checkboxes
		FCheckBoxStyle RedCheckStyle =
			MakeTintableCheckBoxStyle(
				FLinearColor(.20f, 0.10f, 0.10f, 1.f), // background
				FLinearColor(0.85f, 0.25f, 0.25f, 1.f), // border
				1.0f,
				4.0f
			);
		FCheckBoxStyle GreenCheckStyle =
			MakeTintableCheckBoxStyle(
				FLinearColor(.10f, 0.20f, 0.10f, 1.f), // background
				FLinearColor(0.25f, 0.85f, 0.25f, 1.f), // border
				1.0f,
				4.0f
			);

		FCheckBoxStyle BlueCheckStyle =
			MakeTintableCheckBoxStyle(
				FLinearColor(.10f, 0.10f, 0.20f, 1.f), // background
				FLinearColor(0.25f, 0.25f, 0.85f, 1.f), // border
				1.0f,
				4.0f
			);
		
		StyleSet->Set("CheckBox.X", RedCheckStyle);
		StyleSet->Set("CheckBox.Y", GreenCheckStyle);
		StyleSet->Set("CheckBox.Z", BlueCheckStyle);
	}
	
	{ // Panel brushes
		StyleSet->Set("Background.Dark",	new FSlateRoundedBoxBrush(FLinearColor(0.01f, 0.01f, 0.01f, 1.0f), 8.f, FStyleColors::Transparent, 1.f));
	}
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

// END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef IMAGE_PLUGIN_BRUSH
#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef DEFAULT_FONT

void FOhSnapStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}
