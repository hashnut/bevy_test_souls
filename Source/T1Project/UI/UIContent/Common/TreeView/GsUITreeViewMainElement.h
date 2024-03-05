#pragma once


/**
* file		GsUITreeViewMainElement.h
* @brief	GsUITreeViewBaseElement를 상속받은 Main Element
* @author	PKT
* @date		2021/11/23
**/

#include "CoreMinimal.h"
#include "GsUITreeViewBaseElement.h"
#include "GsUITreeViewMainElement.generated.h"


UCLASS()
class T1PROJECT_API UGsUITreeViewMainElement : public UGsUITreeViewBaseElement
{
	GENERATED_BODY()

	const static int32 OPEN_SWITCHER_INDEX = 0;
	const static int32 CLOSE_SWITCHER_INDEX = 1;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual bool IsSelected() const override;


public:	
	virtual void SetSelected(bool InIsSelected);
};