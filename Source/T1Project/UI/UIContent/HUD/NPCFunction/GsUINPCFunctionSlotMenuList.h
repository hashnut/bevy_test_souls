#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"
#include "GsUINPCFunctionSlotMenuList.generated.h"

class UPanelWidget;
class UGsUINPCFunctionSlotMenuItem;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUINPCFunctionSlotMenuList : public UGsUITooltip
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootListItem;
	
private:
	TArray<TWeakObjectPtr<UGsUINPCFunctionSlotMenuItem>> _arrayItem;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetMenuListData(TArray<int> In_arrayNpcTblId);
};