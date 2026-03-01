#include "Widgets/SSnapTransformPrefs.h"

#include "Widgets/OhSnapStyle.h"

void STransformComponent::Construct(const FArguments& Args)
{
	LabelText = Args._Label;
	OptionState = Args._Option;
	OnChanged = Args._OnComponentChanged;

	const FSlateBrush* DarkPanelBrush = FOhSnapStyle::Get()->GetBrush("Background.Dark");

	auto MakeChannelCheckBox = [this](ETransformChannel Channel) -> TSharedRef<SCheckBox>
	{
		bool bIsChecked;
		FName CheckBoxStyleName;
		switch (Channel)
		{
		case ETransformChannel::X:
			bIsChecked = OptionState.X;
			CheckBoxStyleName = "CheckBox.X";
			break;
		case ETransformChannel::Y:
			bIsChecked = OptionState.Y;
			CheckBoxStyleName = "CheckBox.Y";
			break;
		case ETransformChannel::Z:
			bIsChecked = OptionState.Z;
			CheckBoxStyleName = "CheckBox.Z";
			break;
		default:
			bIsChecked = false;
			CheckBoxStyleName = "CheckBox.X";
			break;
		}

		const FCheckBoxStyle& CheckBoxStyle = FOhSnapStyle::Get()->GetWidgetStyle<FCheckBoxStyle>(CheckBoxStyleName);

		TSharedPtr<SCheckBox> CheckBoxWidget;
		SAssignNew(CheckBoxWidget, SCheckBox)
			.IsChecked(bIsChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			.Style(&CheckBoxStyle)
			.IsEnabled(OptionState.bEnabled)
			.OnCheckStateChanged_Lambda([this, Channel](ECheckBoxState NewState)
			{
				this->OnChannelCheckStateChanged(Channel, NewState);
			});

		ChannelCheckBoxes.Add(CheckBoxWidget);
		return CheckBoxWidget.ToSharedRef();
	};

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SCheckBox)
			.IsChecked(OptionState.bEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			.OnCheckStateChanged(this, &STransformComponent::OnSectionCheckStateChanged)
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
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				.HAlign(HAlign_Center)
				[
					MakeChannelCheckBox(ETransformChannel::X)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				.HAlign(HAlign_Center)
				[
					MakeChannelCheckBox(ETransformChannel::Y)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				.HAlign(HAlign_Center)
				[
					MakeChannelCheckBox(ETransformChannel::Z)
				]
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
		.HAlign(HAlign_Fill)
		[
			CreateTransformComponentWidget(ETransformComponent::R)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
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
