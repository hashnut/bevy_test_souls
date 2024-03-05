// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBlockUIController.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Tray/GsUITrayBlockUI.h"
#include "Data/GsLobbyConstant.h"
#include "T1Project.h"

//#define USE_LOG_BLOCK_UI

void UGsBlockUIController::Initialize()
{
	
}

void UGsBlockUIController::Finalize()
{
	
}

void UGsBlockUIController::OnLauncherStart(const UGsLobbyConstant* InData)
{
	if (nullptr == InData)
	{
		// 없으면 비우기. 이 부분을 타면 정상 상황이 아님
		GSLOG(Error, TEXT("[TEST_BLOCK] OnStartWindowIntro(). Cannot Find lobbyConst"));
		_lobbyUIKey = FName(TEXT(""));
		_lobbyUIPath = FSoftObjectPath();
		return;
	}

	_lobbyUIKey = InData->_pathTrayBlockUI.Id;
	_lobbyUIPath = InData->_pathTrayBlockUI.Path;
}

void UGsBlockUIController::Update(float InTick)
{
	// 옵션으로 시간을 세팅했을 경우
	if (0 < _settingLifeRemainSec)
	{
		_settingLifeRemainSec -= InTick;
		if (0 > _settingLifeRemainSec)
		{
			_settingLifeRemainSec = 0;
			Hide();
		}
	}

	// BLOCK_UI_LIFE_SECOND 지나면 BlockUI 강제로 제거함
	if (0 < _lifeRemainSec)
	{
		_lifeRemainSec -= InTick;
		if (0 > _lifeRemainSec)
		{
			_lifeRemainSec = 0;
			ForcedHide(EGsHideBlockUIFactor::LIFE_TIME_OUT);
		}
	}
}

void UGsBlockUIController::SetLobbyUIInfo(const FName& InKey, const FSoftObjectPath& InPath)
{
	_lobbyUIKey = InKey;
	_lobbyUIPath = InPath;
}

void UGsBlockUIController::Show()
{
	++_openCount;

	ShowInter();

#ifdef USE_LOG_BLOCK_UI
	GSLOG(Log, TEXT("[TEST_BLOCK] Show(). count: %d, eCount: %d"), _openCount, _openList.Num());
#endif
}

void UGsBlockUIController::Hide()
{
	_openCount = (0 < _openCount) ? _openCount - 1 : 0;
	
	HideInter();

#ifdef USE_LOG_BLOCK_UI
	GSLOG(Log, TEXT("[TEST_BLOCK] Hide(). count: %d, eCount: %d"), _openCount, _openList.Num());
#endif
}

void UGsBlockUIController::SetSetting(const FText& InMessage, bool bInHideUI, float InLifeSecond)
{
	if (_blockUI.IsValid() && _blockUI->IsVisible())
	{
		if (UGsUITrayBlockUI* trayBalockUI = Cast<UGsUITrayBlockUI>(_blockUI.Get()))
		{
			trayBalockUI->SetTextMessage(InMessage);
			trayBalockUI->SetHideUI(bInHideUI);
		}

		_settingLifeRemainSec = InLifeSecond;
	}
}

void UGsBlockUIController::ShowExplicit(const FName& InKey)
{
	if (_openList.Contains(InKey))
	{
#ifdef USE_LOG_BLOCK_UI
		GSLOG(Warning, TEXT("[TEST_BLOCK] ShowExplicit() 2. count: %d, eCount: %d, key: %s"),
			_openCount, _openList.Num(), *(InKey.ToString()));
#endif
		return;
	}

	_openList.Emplace(InKey);
	
	ShowInter();

#ifdef USE_LOG_BLOCK_UI
	GSLOG(Log, TEXT("[TEST_BLOCK] ShowExplicit() 1. count: %d, eCount: %d, key: %s"),
		_openCount, _openList.Num(), *(InKey.ToString()));
#endif
}

void UGsBlockUIController::HideExplicit(const FName& InKey)
{
	if (false == _openList.Contains(InKey))
	{
#ifdef USE_LOG_BLOCK_UI
		GSLOG(Warning, TEXT("[TEST_BLOCK] HideExplicit() 2. count: %d, eCount: %d, key: %s"),
			_openCount, _openList.Num(), *(InKey.ToString()));
#endif
		return;
	}

	_openList.Remove(InKey);

	HideInter();

#ifdef USE_LOG_BLOCK_UI
	GSLOG(Log, TEXT("[TEST_BLOCK] HideExplicit() 1. count: %d, eCount: %d, key: %s"),
		_openCount, _openList.Num(), *(InKey.ToString()));
#endif
}

void UGsBlockUIController::ForcedHide(EGsHideBlockUIFactor InFactor)
{
	_openCount = 0;
	_openList.Empty();
	_lifeRemainSec = 0;
	_settingLifeRemainSec = 0;

	HideInter();

#ifdef USE_LOG_BLOCK_UI
	GSLOG(Log, TEXT("[TEST_BLOCK] ForcedHide(). factor: %d"), static_cast<int32>(InFactor));
#endif
}

void UGsBlockUIController::ShowInter()
{
	if (false == _blockUI.IsValid())
	{
		if (false == _lobbyUIKey.IsNone())
		{
			// 한 번만 꽂아두고 재활용한다. UIController가 관리하므로 포인터만 얻어쓴다
			_blockUI = GUI()->OpenAndGetWidgetByPath(_lobbyUIKey, _lobbyUIPath);

#ifdef USE_LOG_BLOCK_UI
			GSLOG(Warning, TEXT("[TEST_BLOCK] ShowInter() AddUI"));
#endif
		}
	}

	// 그래도 이상하면 리턴처리
	if (false == _blockUI.IsValid())
	{
		GSLOG(Error, TEXT("[TEST_BLOCK] ShowInter() Invalid _blockUI"));
		return;
	}

	// 주의: 순서에 유의. Visibility 호출 전에 부를 것
	if (UGsUITrayBlockUI* trayBalockUI = Cast<UGsUITrayBlockUI>(_blockUI.Get()))
	{
		trayBalockUI->Show();
	}
	
	_lifeRemainSec = BLOCK_UI_LIFE_SECOND;
	_settingLifeRemainSec = 0; // 새로 들어올 경우 리셋함
}

void UGsBlockUIController::HideInter()
{
	if (0 < _openList.Num())
	{
		return;
	}

	if (false == _blockUI.IsValid())
	{
		return;
	}

	if (UGsUITrayBlockUI* trayBalockUI = Cast<UGsUITrayBlockUI>(_blockUI.Get()))
	{
		trayBalockUI->Hide();
	}
	
	_lifeRemainSec = 0;
	_settingLifeRemainSec = 0;
}

bool UGsBlockUIController::IsOpened() const
{
	if (0 < _openList.Num())
	{
		if (_blockUI.IsValid())
		{
			return true;
		}
	}

	return false;
}

void UGsBlockUIController::PrintLog()
{
//#ifdef USE_LOG_BLOCK_UI
	GSLOG(Log, TEXT("[TEST_BLOCK] PrintLog: openCount: %d, eCount: %d"), _openCount, _openList.Num());

	for (int i = 0; i < _openList.Num(); ++i)
	{
		GSLOG(Log, TEXT("[TEST_BLOCK] PrintLog: block name: %s"), *(_openList[i].ToString()));
	}
//#endif
}
