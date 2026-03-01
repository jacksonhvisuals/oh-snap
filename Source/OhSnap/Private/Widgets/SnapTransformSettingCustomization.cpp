// Copyright Jackson Hayes. All Rights Reserved.

#include "Widgets/SnapTransformSettingCustomization.h"

#include "Containers/Array.h"
#include "Delegates/Delegate.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Engine/ImportantToggleSettingInterface.h"
#include "Fonts/SlateFontInfo.h"
#include "HAL/PlatformProcess.h"
#include "IDetailPropertyRow.h"
#include "OhSnapSettings.h"
#include "OhSnapTypes.h"
#include "OhSnapUtilities.h"
#include "Layout/Children.h"
#include "Layout/Margin.h"
#include "Misc/Attribute.h"
#include "PropertyHandle.h"
#include "SlotBase.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateTypes.h"
#include "Templates/Casts.h"
#include "Types/SlateEnums.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SSnapTransformPrefs.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SnapTransformSettingCustomization"

TSharedRef<IDetailCustomization> FSnapTransformSettingCustomization::MakeInstance()
{
	return MakeShareable(new FSnapTransformSettingCustomization);
}

void FSnapTransformSettingCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);

	if (Objects.Num() == 1)
	{
		TransformSettingObject = Objects[0];
		
		UOhSnapSettings* OhSnapSettingsObject = Cast<UOhSnapSettings>(TransformSettingObject.Get());

		if (IsValid(OhSnapSettingsObject))
		{
			FSnapTransformOptions Options = OhSnapSettingsObject->SnapTransformOptions;
			
			FName CategoryName = "Actor to Actor";
			FName PropertyName = "SnapTransformOptions";

			IDetailCategoryBuilder& Category = DetailBuilder.EditCategory(CategoryName);
			TransformPropertyHandle = DetailBuilder.GetProperty(PropertyName);

			FSlateFontInfo StateDescriptionFont = IDetailLayoutBuilder::GetDetailFont();
			StateDescriptionFont.Size += 4;

			// Customize snap transform options section
			Category.InitiallyCollapsed(false)
			.AddProperty(TransformPropertyHandle)
			.ShouldAutoExpand(true)
			.CustomWidget()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 6.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SSnapTransformPreferences)
						.TransformOptions(Options)
						.OnOptionsChanged_Lambda([](const FSnapTransformOptions& NewOptions)
						{
							OhSnapUtils::SaveSettings(NewOptions);
						})
					]
				]
			];			
		}
	}


}

#undef LOCTEXT_NAMESPACE
