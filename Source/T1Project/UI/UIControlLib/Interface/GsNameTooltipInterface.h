#pragma once

#include "CoreMinimal.h"

#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"

#include "UI/UIContent/Tray/GsUITrayItemNameToolTip.h"

#include "GsNameTooltipInterface.generated.h"

/**
 * 
 */
UINTERFACE(meta=( CannotImplementInterfaceInBlueprint ))
class T1PROJECT_API UGsNameTooltipInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


class T1PROJECT_API IGsNameTooltipInterface
{
	GENERATED_IINTERFACE_BODY()

protected:
	TWeakObjectPtr<UGsUITrayItemNameToolTip> _nameToolTipObjPtr;


public:
	virtual void OpenNameTooltip() {}
	virtual void CloseNameTooltip();
};
