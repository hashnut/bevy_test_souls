#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "GsIconSelectorInterface.generated.h"

/**
 * 아이콘 생성기능을 부여할 위젯의 ContentWidget의 인터페이스
 */
UINTERFACE(meta=( CannotImplementInterfaceInBlueprint ))
class T1PROJECT_API UGsIconSelectorInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


class T1PROJECT_API IGsIconSelectorInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual class UGsUIIconBase* CreateIcon(EGsIconType InIconType, uint8 InIconKey) = 0;
	virtual class UGsUIIconBase* CreateIcon() = 0;
	virtual class UPanelWidget* GetPanel() = 0;
	virtual void WarmUp(int32 InWarmUpIconCount) = 0;

public:
	// 모든 자식 슬롯 제거, 아이콘 풀 반납을 진행
	void RemoveAllChildren();
};
