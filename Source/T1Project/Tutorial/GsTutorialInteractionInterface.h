#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "GsTutorialInteractionInterface.generated.h"

/**
 * 
 */
UINTERFACE(meta=( CannotImplementInterfaceInBlueprint ))
class T1PROJECT_API UGsTutorialInteractionInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


class T1PROJECT_API IGsTutorialInteractionInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	DECLARE_DELEGATE(FGsOnTutorialInteraction);

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) = 0;
	virtual bool IsTutorialInteraction() const = 0;
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() = 0;

	// CLICK_REGION_INTERFACE_EVENT 시에만 구현
public:
	virtual void OnClickTutorialRegion() {}
};
