#pragma once
#include "Templates/Tuple.h"
#include "OhSnapTypes.generated.h"

UENUM()
enum class ETransformChannel : uint8
{
	X,
	Y,
	Z
};

UENUM()
enum class ETransformComponent : uint8
{
	T,
	R,
	S
};

USTRUCT()
struct FTransformComponentOption
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool X = true;
	
	UPROPERTY()
	bool Y = true;
	
	UPROPERTY()
	bool Z = true;
};

USTRUCT()
struct FSnapTransformOptions
{
	GENERATED_BODY()
	
	UPROPERTY()
	FTransformComponentOption Translation = {true, true, true};
	UPROPERTY()
	FTransformComponentOption Rotation = {true, true, true};
	UPROPERTY()
	FTransformComponentOption Scale = {false, false, false};
};

DECLARE_DELEGATE_OneParam(FOnTransformComponentChanged, const FTransformComponentOption&);
DECLARE_DELEGATE_OneParam(FOnTransformOptionsChanged, const FSnapTransformOptions&);
