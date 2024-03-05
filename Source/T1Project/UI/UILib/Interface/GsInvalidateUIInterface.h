#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "GsInvalidateUIInterface.generated.h"

/**
 * 중복갱신을 방지하기 위한 로직
 */
UINTERFACE(meta=( CannotImplementInterfaceInBlueprint ))
class T1PROJECT_API UGsInvalidateUIInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


class T1PROJECT_API IGsInvalidateUIInterface
{
	GENERATED_IINTERFACE_BODY()

protected:
	bool _bDirty = false;

protected:
	// 상속 클래스에서 구현. UI갱신을 위한 코드만 넣을 것.
	virtual void InvalidateAllInternal() = 0;

public:
	// 갱신 플래그 On
	void InvalidateAll();

	// Tick 함수에 연결
	void CheckInvalidate();

	// _bDirty 변수 접근자
	void SetDirtyInvalidate(bool InDirty);
	bool GetDirtyInvalidate() const { return _bDirty; }
};
