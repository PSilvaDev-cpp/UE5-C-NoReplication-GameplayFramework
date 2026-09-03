// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define ECC_AbilityTrace ECC_GameTraceChannel1
#define ECC_ShootTrace ECC_GameTraceChannel2

class FSinglePlayerModularGameplayFrameworkModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
