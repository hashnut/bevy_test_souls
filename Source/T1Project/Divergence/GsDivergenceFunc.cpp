#include "GsDivergenceFunc.h"

#include "EngineSettings/Classes/GeneralProjectSettings.h"

#if PLATFORM_ANDROID
#include "Android/AndroidRuntimeSettings/Classes/AndroidRuntimeSettings.h"
#endif

#if PLATFORM_IOS
#include "IOS/IOSRuntimeSettings/Classes/IOSRuntimeSettings.h"
#endif
// 버전 가져오기
FString FGsDivergenceFunc::GetVersion()
{
	FString versionText;
#if PLATFORM_ANDROID 
	const UAndroidRuntimeSettings* Settings = GetDefault<UAndroidRuntimeSettings>();
	if (Settings)
	{
		versionText = Settings->VersionDisplayName;
	}
#elif PLATFORM_IOS
	const UIOSRuntimeSettings* Settings = GetDefault<UIOSRuntimeSettings>();
	if (Settings)
	{
		versionText = Settings->VersionInfo;
	}
#elif WITH_EDITOR || PLATFORM_WINDOWS
	const UGeneralProjectSettings* Settings = GetDefault<UGeneralProjectSettings>();
	if (Settings)
	{
		versionText = Settings->ProjectVersion;
	}
#endif
	return versionText;
}