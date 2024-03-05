#pragma once


/**
* file		GsUITreeViewMainElement.h
* @brief	GsUITreeViewBaseElement를 상속받은 Sub Element
* @author	PKT
* @date		2021/11/23
**/

#include "CoreMinimal.h"
#include "GsUITreeViewBaseElement.h"
#include "GsUITreeViewSubElement.generated.h"


UCLASS()
class T1PROJECT_API UGsUITreeViewSubElement : public UGsUITreeViewBaseElement
{
	GENERATED_BODY()

	const static int32 NORMAL_SWITCHER_INDEX = 0;
	const static int32 SELECTED_SWITCHER_INDEX = 1;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	bool IsSelected() const override;

public:
	virtual void SetSelected(bool InIsSelected) override;
};