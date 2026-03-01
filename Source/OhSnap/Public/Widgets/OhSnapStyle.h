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
	
	static FCheckBoxStyle MakeTintableCheckBoxStyle(
		const FLinearColor& Background,
		const FLinearColor& Border,
		float              BorderWidth = 1.f,
		float              Radius      = 4.f
	)
	{
		FCheckBoxStyle Style =
			FAppStyle::Get().GetWidgetStyle<FCheckBoxStyle>("CheckBox");
		
		const FVector4 R(Radius);

		Style.SetCheckBoxType(ESlateCheckBoxType::CheckBox);

		Style.SetUncheckedImage(
			FSlateRoundedBoxBrush(Background, R, Border, BorderWidth));

		Style.SetUncheckedHoveredImage(
			FSlateRoundedBoxBrush(Background * 1.1f, R, Border, BorderWidth));

		Style.SetUncheckedPressedImage(
			FSlateRoundedBoxBrush(Background * 0.9f, R, Border, BorderWidth));

		Style.SetCheckedImage(
			FSlateRoundedBoxBrush(Background, R, Border, BorderWidth));

		Style.SetCheckedHoveredImage(
			FSlateRoundedBoxBrush(Background * 1.1f, R, Border, BorderWidth));

		Style.SetCheckedPressedImage(
			FSlateRoundedBoxBrush(Background * 0.9f, R, Border, BorderWidth));

		return Style;
	}
private:
	static TSharedPtr< class FSlateStyleSet > StyleSet;
};
