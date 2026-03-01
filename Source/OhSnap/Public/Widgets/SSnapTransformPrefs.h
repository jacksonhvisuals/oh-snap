#pragma once
#include "OhSnapTypes.h"

class STransformComponent : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(STransformComponent) {}
		SLATE_ARGUMENT(FText, Label)
		SLATE_ARGUMENT(FTransformComponentOption, Option)
		SLATE_EVENT(FOnTransformComponentChanged, OnComponentChanged)
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& Args);
private:
	void OnSectionCheckStateChanged(ECheckBoxState NewState);
	void OnChannelCheckStateChanged(ETransformChannel Channel, ECheckBoxState NewState);
	void BroadcastChange();

	FText LabelText;
	FTransformComponentOption OptionState;
	FOnTransformComponentChanged OnChanged;
	TArray<TSharedPtr<SCheckBox>> ChannelCheckBoxes;
};

class SSnapTransformPreferences : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSnapTransformPreferences) {}
		SLATE_ARGUMENT(FSnapTransformOptions, TransformOptions)
		SLATE_EVENT(FOnTransformOptionsChanged, OnOptionsChanged)
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& Args);
private:
	TSharedRef<SWidget> CreateTransformComponentWidget(ETransformComponent ComponentType);
private:
	FSnapTransformOptions Options;
	FOnTransformOptionsChanged OnOptionsChanged;
};
