#pragma once

#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"

#include "GsPlatformManager.generated.h"


//------------------------------------------------------------------------------
// 플렛폼별 관리 Base 매니저 
//------------------------------------------------------------------------------
UCLASS()
class T1PROJECT_API UGsPlatformManager :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;
	virtual void Finalize() override;
};

#define GPlatform() UGsScopeHolder::GetGlobalManager<UGsPlatformManager>(UGsScopeGlobal::EManagerType::Platform)

