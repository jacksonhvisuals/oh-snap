// Jackson Hayes 2025 All Rights Reserved.

#pragma once
#include "OhSnapTypes.h"

class OHSNAP_API FOhSnapCallbacks
{
public:
	static void SnapActorToActor(FSnapTransformOptions Options);
	static bool SnapActorToActor_CanExecute();
};
