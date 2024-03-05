#pragma once


#include "CoreMinimal.h"
#include "Core/Public/Modules/ModuleInterface.h"


class FGsAnimGraphExModule : public IModuleInterface
{
public:
	/** IModuleInterface interface */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
