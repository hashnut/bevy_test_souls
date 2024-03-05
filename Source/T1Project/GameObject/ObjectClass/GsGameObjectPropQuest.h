
#pragma once

#include "GsGameObjectPropTouch.h"
#include "GsGameObjectPropQuest.generated.h"

/**
* 침공전 보물 인터랙션 프랍
**/

UCLASS()
class T1PROJECT_API UGsGameObjectPropQuest : public UGsGameObjectPropTouch
{
	GENERATED_BODY()

public:
	virtual void SetQuestTargetActive(bool In_isActive, int In_Index = 0) override;
	virtual void InvalidWidget() override;
	virtual void SendInteractionStart(const struct FGsPropInteractionMessageParamContents* inParam) override;
	virtual void SendInteractionEnd(const struct FGsPropInteractionMessageParamContents* inParam) override;
	virtual bool CheckNeedToDeactiveInteractionWidget() override;
};