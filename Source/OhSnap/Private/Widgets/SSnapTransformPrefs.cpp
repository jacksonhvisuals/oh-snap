#include "Widgets/SSnapTransformPrefs.h"

void STransformComponent::Construct(const FArguments& Args)
{
	LabelText = Args._Label;
	OptionState = Args._Option;
	OnChanged = Args._OnComponentChanged;
	
	auto Checkbox = [this](ETransformChannel Channel) -> TSharedRef<SCheckBox>
	{
		ECheckBoxState IsChecked;
		switch (Channel)
		{
		case ETransformChannel::X:
			IsChecked = (this->OptionState.X == true) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
		case ETransformChannel::Y:
			IsChecked = (this->OptionState.Y == true) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
		case ETransformChannel::Z:
			IsChecked = (this->OptionState.Z == true) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
		}
		
		TSharedRef<SCheckBox> CheckBoxWidget = SNew(SCheckBox)
			.IsChecked(IsChecked)
			.OnCheckStateChanged_Lambda([this, Channel](ECheckBoxState NewState) 
			{
				switch (Channel)
				{
				case ETransformChannel::X:
					this->OptionState.X = (NewState == ECheckBoxState::Checked);
				case ETransformChannel::Y:
					this->OptionState.Y = (NewState == ECheckBoxState::Checked);
				case ETransformChannel::Z:
					this->OptionState.Z = (NewState == ECheckBoxState::Checked);
				}
			});
		
		return CheckBoxWidget;
	};
	
	ChildSlot
	[
		SNew(SBox)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(LabelText)
			]
			+SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					Checkbox(ETransformChannel::X)
				]
				+ SHorizontalBox::Slot()
				[
					Checkbox(ETransformChannel::Y)
				]
				+ SHorizontalBox::Slot()
				[
					Checkbox(ETransformChannel::Z)
				]
			]
		]
	];
}

// UOhSnapSettings* Settings = GetMutableDefault<UOhSnapSettings>();
void SSnapTransformPreferences::Construct(const FArguments& Args)
{
	ChildSlot
		[
			SNew(SBox)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					CreateTransformComponentWidget((ETransformComponent::T))
				]
				+ SVerticalBox::Slot()
				[
					CreateTransformComponentWidget((ETransformComponent::R))
				]
				+ SVerticalBox::Slot()
				[
					CreateTransformComponentWidget((ETransformComponent::S))
				]
			]
		];
}

TSharedRef<SWidget> SSnapTransformPreferences::CreateTransformComponentWidget(ETransformComponent ComponentType)
{
	FText ChannelName;
	switch (ComponentType)
	{
	case ETransformComponent::T:
		ChannelName = FText::FromString("Translation");
		break;
	case ETransformComponent::R:
		ChannelName = FText::FromString("Rotation");
		break;
	case ETransformComponent::S:
		ChannelName = FText::FromString("Scale");
		break;
	};
	
	return SNew(STransformComponent)
		.Label(ChannelName)
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
			};
		});
}
