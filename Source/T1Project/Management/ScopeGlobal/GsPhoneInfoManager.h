#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
//-----------------------------------------------------------------------
// 폰정보 관리 매니저
//-----------------------------------------------------------------------

class FMOSPhoneInfoBase;

class T1PROJECT_API FGsPhoneInfoManager :
	public IGsManager
{
	// 멤버 변수
private:
	FMOSPhoneInfoBase* _phoneInfo;

	// 소멸자
public:
	virtual ~FGsPhoneInfoManager();

	// 가상함수
public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	// get
public:
	// 폰정보 얻어오기
#if PLATFORM_ANDROID || PLATFORM_IOS
	FMOSPhoneInfoBase* GetPhoneInfo() { return _phoneInfo; }
#endif
};


#define GPhoneInfo() UGsScopeHolder::GetGlobalManagerFType<FGsPhoneInfoManager>(UGsScopeGlobal::EManagerType::PhoneInfo)