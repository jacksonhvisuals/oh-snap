#include "Widgets/SSnapTransformPrefs.h"

#include "Misc/AxisDisplayInfo.h"
#include "Styling/AppStyle.h"
#include "Widgets/OhSnapStyle.h"

void STransformComponent::Construct(const FArguments& Args)
{
	LabelText = Args._Label;
	OptionState = Args._Option;
	OnChanged = Args._OnComponentChanged;

	const FSlateBrush* DarkPanelBrush = FOhSnapStyle::Get()->GetBrush("Background.Dark");
	const FSlateBrush* NarrowDecoratorBrush = FAppStyle::Get().GetBrush("NumericEntrySpinBox.NarrowDecorator");

	auto MakeChannelCheckBox = [this, NarrowDecoratorBrush](ETransformChannel Channel) -> TSharedRef<SWidget>
	{
		bool bIsChecked;
		FText AxisLabel;
		FLinearColor AxisColor;
		switch (Channel)
		{
		case ETransformChannel::X:
			bIsChecked = OptionState.X;
			AxisLabel = FText::FromString("X");
			AxisColor = AxisDisplayInfo::GetAxisColor(EAxisList::X);
			break;
		case ETransformChannel::Y:
			bIsChecked = OptionState.Y;
			AxisLabel = FText::FromString("Y");
			AxisColor = AxisDisplayInfo::GetAxisColor(EAxisList::Y);
			break;
		case ETransformChannel::Z:
			bIsChecked = OptionState.Z;
			AxisLabel = FText::FromString("Z");
			AxisColor = AxisDisplayInfo::GetAxisColor(EAxisList::Z);
			break;
		default:
			bIsChecked = false;
			AxisLabel = FText::FromString("X");
			AxisColor = AxisDisplayInfo::GetAxisColor(EAxisList::X);
			break;
		}

		TSharedPtr<SCheckBox> CheckBoxWidget;
		SAssignNew(CheckBoxWidget, SCheckBox)
			.IsChecked(bIsChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			.IsEnabled(OptionState.bEnabled)
			.OnCheckStateChanged_Lambda([this, Channel](ECheckBoxState NewState)
			{
				this->OnChannelCheckStateChanged(Channel, NewState);
			});

		ChannelCheckBoxes.Add(CheckBoxWidget);

		return SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBorder)
				.Visibility(EVisibility::HitTestInvisible)
				.BorderImage(NarrowDecoratorBrush)
				.BorderBackgroundColor(AxisColor)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				CheckBoxWidget.ToSharedRef()
			];
	};

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(0, 6)
		.AutoHeight()
		[
			SNew(SCheckBox)
			.IsChecked(OptionState.bEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			.OnCheckStateChanged(this, &STransformComponent::OnSectionCheckStateChanged)
			.Padding(4.f)
			.Content()
			[
				SNew(STextBlock)
				.Text(LabelText)
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		[
			SNew(SBorder)
			.BorderImage(DarkPanelBrush)
			.HAlign(HAlign_Fill)
			.Padding(12.f)
			[
				[&]() -> TSharedRef<SWidget>
				{
					if (Args._bCompactChannels)
					{
						return SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().AutoWidth().Padding(6.f, 0.f)
							[ MakeChannelCheckBox(ETransformChannel::X) ]
							+ SHorizontalBox::Slot().AutoWidth().Padding(6.f, 0.f)
							[ MakeChannelCheckBox(ETransformChannel::Y) ]
							+ SHorizontalBox::Slot().AutoWidth().Padding(6.f, 0.f)
							[ MakeChannelCheckBox(ETransformChannel::Z) ];
					}
					return SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.f).HAlign(HAlign_Center)
						[ MakeChannelCheckBox(ETransformChannel::X) ]
						+ SHorizontalBox::Slot().FillWidth(1.f).HAlign(HAlign_Center)
						[ MakeChannelCheckBox(ETransformChannel::Y) ]
						+ SHorizontalBox::Slot().FillWidth(1.f).HAlign(HAlign_Center)
						[ MakeChannelCheckBox(ETransformChannel::Z) ];
				}()
			]
		]
	];
}

void STransformComponent::OnSectionCheckStateChanged(ECheckBoxState NewState)
{
	OptionState.bEnabled = (NewState == ECheckBoxState::Checked);
	for (TSharedPtr<SCheckBox>& CB : ChannelCheckBoxes)
	{
		if (CB.IsValid())
		{
			CB->SetEnabled(OptionState.bEnabled);
		}
	}
	BroadcastChange();
}

void STransformComponent::OnChannelCheckStateChanged(ETransformChannel Channel, ECheckBoxState NewState)
{
	const bool bChecked = (NewState == ECheckBoxState::Checked);
	switch (Channel)
	{
	case ETransformChannel::X: OptionState.X = bChecked; break;
	case ETransformChannel::Y: OptionState.Y = bChecked; break;
	case ETransformChannel::Z: OptionState.Z = bChecked; break;
	}
	BroadcastChange();
}

void STransformComponent::BroadcastChange()
{
	OnChanged.ExecuteIfBound(OptionState);
}

void SSnapTransformPreferences::Construct(const FArguments& Args)
{
	Options = Args._TransformOptions;
	OnOptionsChanged = Args._OnOptionsChanged;
	bCompactChannels = Args._bCompactChannels;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		[
			CreateTransformComponentWidget(ETransformComponent::T)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		.HAlign(HAlign_Fill)
		[
			CreateTransformComponentWidget(ETransformComponent::R)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 8, 0)
		.HAlign(HAlign_Fill)
		[
			CreateTransformComponentWidget(ETransformComponent::S)
		]
	];
}

TSharedRef<SWidget> SSnapTransformPreferences::CreateTransformComponentWidget(ETransformComponent ComponentType)
{
	FText ChannelName;
	FTransformComponentOption InitialOption;
	switch (ComponentType)
	{
	case ETransformComponent::T:
		ChannelName = FText::FromString("Translation");
		InitialOption = Options.Translation;
		break;
	case ETransformComponent::R:
		ChannelName = FText::FromString("Rotation");
		InitialOption = Options.Rotation;
		break;
	case ETransformComponent::S:
		ChannelName = FText::FromString("Scale");
		InitialOption = Options.Scale;
		break;
	default:
		ChannelName = FText::FromString("Unknown");
		break;
	}

	return SNew(STransformComponent)
		.Label(ChannelName)
		.Option(InitialOption)
		.bCompactChannels(bCompactChannels)
		.OnComponentChanged_Lambda([this, ComponentType](const FTransformComponentOption& NewOption)
		{
			switch (ComponentType)
			{
			case ETransformComponent::T:
				this->Options.Translation = NewOption;
				break;
			case ETransformComponent::R:
				this->Options.Rotation = NewOption;
				break;
			case ETransformComponent::S:
				this->Options.Scale = NewOption;
				break;
			}
			this->OnOptionsChanged.ExecuteIfBound(this->Options);
		});
}
