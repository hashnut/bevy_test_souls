#pragma once

#include "GsPlatformManager.h"

#if PLATFORM_ANDROID
#endif

#include "GsAndroidPlatformManager.generated.h"

//------------------------------------------------------------------------------
// 플렛폼별 관리 매니저 (Android)
//------------------------------------------------------------------------------
UCLASS()
class T1PROJECT_API UGsAndroidPlatformManager :
	public UGsPlatformManager
{
	GENERATED_BODY()

#if PLATFORM_ANDROID
public:
	virtual void Initialize() override;
	virtual void Finalize() override;
#endif
};