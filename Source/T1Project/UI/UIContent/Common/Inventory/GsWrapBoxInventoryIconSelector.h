#pragma once

#include "CoreMinimal.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "GsWrapBoxInventoryIconSelector.generated.h"

/**
* file		GsWrapBoxInventoryIconSelector.h
* @brief	GsWrapBoxIconSelector 에서 네이티브 에서 설정 가능하게 기능 추가
* @author	PKT
* @date		2021/07/21
**/
UCLASS()
class T1PROJECT_API UGsWrapBoxInventoryIconSelector : public UGsWrapBoxIconSelector
{
	GENERATED_BODY()

public:
	void SetEIconType(EGsIconType InType = EGsIconType::NONE);
	void SetIconKey(uint8 InIconKey = 0);
	void SetpreviewCount(int32 InPreviewCount = 0);
};
