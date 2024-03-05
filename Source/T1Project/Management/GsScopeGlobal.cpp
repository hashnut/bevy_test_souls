// Fill out your copyright notice in the Description page of Project Settings.
#include "GsScopeGlobal.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsResourceManager.h"

#include "GameObject/GsGlobalGameObjects.h"
#include "Item/GsItemManager.h"
#include "Sequence/GsSequenceManager.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsPhoneInfoManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsPatchManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsDataCenterManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsOfflinePlayManager.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"
#include "Management/ScopeGlobal/GsPopupSystemManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGlobal/Platform/GsWindowPlatformManager.h"
#include "Management/ScopeGlobal/Platform/GsIosPlatformManager.h"
#include "Management/ScopeGlobal/Platform/GsAndroidPlatformManager.h"

void UGsScopeGlobal::Create()
{
	UGsScopeBase::Create();

	// fType이 먼저 선언되어야 할수 밖에 없는 경우(Enum 순서가 결국 초기화 순서)에 인덱스 맞추기위한 플레그
	_uTypeFirstOrder = false;
	_fTypeCount = EManagerType::FObjectMgrMax;

	// EManagerType 열거형을 TArray의 접근 index로 사용하기에, 열거형과 Manager의 생성 순서 보장이 필수적이다
	// CreateManager를 열거의 순서대로 호출해도 문제가 발생하지 않지만, 사람을 믿는 것보단 이게 더 안전하다
	for (uint8 mgrType = 0; mgrType < EManagerType::Max; mgrType++)
	{
		switch (mgrType)
		{
		case EManagerType::Path:
			CreateUObjectManager<UGsPathManager>(this);
			break;
		case EManagerType::Resource:
			CreateUObjectManager<UGsResourceManager>(this);
			break;
		case EManagerType::Data:
			CreateUObjectManager<UGsDataContainManager>(this);
			break;
		case EManagerType::GameFlow:
			CreateFObjectManager<FGsGameFlowManager>();
			break;
		case EManagerType::GGameObjects:
			CreateUObjectManager<UGsGlobalGameObjects>(this);
			break;
		case EManagerType::Net:
			CreateFObjectManager<FGsNetManager>();
			break;
		case EManagerType::Item:
			CreateUObjectManager<UGsItemManager>(this);
			break;
		case EManagerType::Skill:
			CreateUObjectManager<UGsSkillManager>(this);
			break;
		case EManagerType::Ui:
			CreateUObjectManager<UGsUIManager>(this);
			break;
		case EManagerType::Level:
			CreateUObjectManager<UGsLevelManager>(this);
			break;
		case EManagerType::Sequence:
			CreateUObjectManager<UGsSequenceManager>(this);
			break;
		case EManagerType::Sound:
			CreateUObjectManager<UGsSoundManager>(this);
			break;
		case EManagerType::PhoneInfo:
			CreateFObjectManager<FGsPhoneInfoManager>();
			break;
		case EManagerType::Option:
			CreateUObjectManager<UGsOptionManager>(this);
			break;
		case EManagerType::GameData:
			CreateFObjectManager<FGsGameDataManager>();
			break;			
		case EManagerType::DataCenter:
			CreateFObjectManager<FGsDataCenterManager>();
			break;
		case EManagerType::Guild:
			CreateFObjectManager<FGsGuildManager>();
			break;		
		case EManagerType::Localization:
			CreateUObjectManager<UGsLocalizationManager>(this);
			break;
		case EManagerType::Hive:
			CreateUObjectManager<UGsHiveManager>(this);
			break;
		case EManagerType::Divergence:
			CreateUObjectManager<UGsDivergenceManager>(this);
			break;
		case EManagerType::Patch:
			CreateUObjectManager<UGsPatchManager>(this);
			break;
		case EManagerType::OfflinePlay:
			CreateFObjectManager<FGsOfflinePlayManager>();
			break;
		case EManagerType::PopupSystem:
			CreateUObjectManager<UGsPopupSystemManager>(this);
			break;
		case EManagerType::Quest:
			CreateUObjectManager<UGsQuestManager>(this);
			break;
		case EManagerType::BMShop:
			CreateFObjectManager<FGsBMShopManager>();
			break;
		case EManagerType::Platform:
#if PLATFORM_WINDOWS
			CreateUObjectManager<UGsWindowPlatformManager>(this);
#elif PLATFORM_IOS
			CreateUObjectManager<UGsIosPlatformManager>(this);
#elif PLATFORM_ANDROID
			CreateUObjectManager<UGsAndroidPlatformManager>(this);
#else
#endif
			break;
		default:
			check(false);
			break;
		}
	}
}

void UGsScopeGlobal::Init()
{
	Super::Init();
}

void UGsScopeGlobal::InitAfterResourceDownload()
{
	Super::InitAfterResourceDownload();
}
