#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUICustomErrorPopup.generated.h"

UCLASS()
class  T1PROJECT_API UGsUICustomErrorPopup : public UGsUIPopup
{
	GENERATED_BODY()

public:
	virtual void SetErrorHandlerData(FText useText, int optionVal, uint16 result = 0) {}
};