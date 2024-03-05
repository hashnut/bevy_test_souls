#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "GsEntryInterface.generated.h"

/**
 * 
 */
UINTERFACE(meta=( CannotImplementInterfaceInBlueprint ))
class T1PROJECT_API UGsEntryInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


class T1PROJECT_API IGsEntryInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void OnShow() {}
	virtual void OnHide() {}
};
