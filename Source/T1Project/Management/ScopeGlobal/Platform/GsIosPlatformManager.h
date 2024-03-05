#pragma once

#include "GsPlatformManager.h"

#if PLATFORM_IOS
#endif

#include "GsIosPlatformManager.generated.h"

//------------------------------------------------------------------------------
// 플렛폼별 관리 매니저 (Ios)
//------------------------------------------------------------------------------
UCLASS()
class T1PROJECT_API UGsIosPlatformManager :
	public UGsPlatformManager
{
	GENERATED_BODY()

#if PLATFORM_IOS
public:
	virtual void Initialize() override;
	virtual void Finalize() override;
#endif
};
