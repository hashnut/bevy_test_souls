#include "GsPhoneInfoManager.h"
#if PLATFORM_ANDROID
#include "MOSPhoneInfoAndroid.h"
#elif PLATFORM_IOS
#include "MOSPhoneInfoIOS.h"
#endif

#include "MOSPhoneInfoBase.h"

FGsPhoneInfoManager::~FGsPhoneInfoManager()
{	
}

void FGsPhoneInfoManager::Initialize()
{
#if PLATFORM_ANDROID
	_phoneInfo = new FMOSPhoneInfoAndroid();
#elif PLATFORM_IOS
	_phoneInfo = new FMOSPhoneInfoIOS();
//#else
//	_phoneInfo = new FMOSPhoneInfoBase();
#endif


}
void FGsPhoneInfoManager::Finalize()
{
	if (_phoneInfo != nullptr)
	{
		delete _phoneInfo;
		_phoneInfo = nullptr;
	}
}