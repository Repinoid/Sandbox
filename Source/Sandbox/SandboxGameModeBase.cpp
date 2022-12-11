// Copyright Epic Games, Inc. All Rights Reserved.
#include "SandboxGameModeBase.h"
#include "SandboxPawn.h"

ASandboxGameModeBase::ASandboxGameModeBase()
{
	DefaultPawnClass = ASandboxPawn::StaticClass();
}
