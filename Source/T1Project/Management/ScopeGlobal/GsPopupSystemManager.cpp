// Fill out your copyright notice in the Description page of Project Settings.


#include "Management/ScopeGlobal/GsPopupSystemManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "UI/UIContent/Popup/GsUIPopupSystemYesNo.h"
#include "UI/UIContent/Popup/GsUIPopupSystem.h"
#include "Data/GsLobbyConstant.h"
#include "T1Project.h"


void UGsPopupSystemManager::Initialize()
{
	/*if (!_popupSystemUI.IsValid())
	{
		_popupSystemUI = GetPopupSystemUI();
	}*/
}

void UGsPopupSystemManager::Finalize()
{
	if (_popupSystemUI.IsValid())
	{
		_popupSystemUI->Close();
	}
}

void UGsPopupSystemManager::OnLauncherStart()
{
	if (UGsLobbyDataManager* lobbyMgr = LSLobbyData())
	{
		if (const UGsLobbyConstant* lobbyConst = lobbyMgr->GetLobbyConstantData())
		{
			_popupOkKey = lobbyConst->_pathPopupSystem.Id;
			_popupOkPath = lobbyConst->_pathPopupSystem.Path;

			_popupYesNoKey = lobbyConst->_pathPopupSystemYesNo.Id;
			_popupYesNoPath = lobbyConst->_pathPopupSystemYesNo.Path;

			return;
		}
	}

	// 없으면 비우기. 이 부분을 타면 정상 상황이 아님
	GSLOG(Error, TEXT("[POPUP_SYSTEM_MANAGER] OnLauncherStart(). Cannot Find lobbyConst"));

	_popupOkKey = FName(TEXT(""));
	_popupOkPath = FSoftObjectPath();
	_popupYesNoKey = FName(TEXT(""));
	_popupYesNoPath = FSoftObjectPath();
}

void UGsPopupSystemManager::RemoveAll()
{
	for (auto iter = _popupInfoList.CreateConstIterator(); iter; ++iter)
	{
		TSharedPtr<FPopupInfo> popupInfo = iter->ToSharedRef();
		if (nullptr != popupInfo && popupInfo.IsValid())
		{
			if (popupInfo->GetButtonType() == EPopupButtonType::Yes)
			{
				// 팝업을 모두 닫아야 하는경우에도 FUNC을 수행해야하는가??
				// 수행안했을때 문제점은? 2022-05-20
				/*const TFunction<void()>& func = popupInfo->GetCallbackYesFunc();
				if (func)
				{
					func();
				}*/
			}
			else
			{
				const TFunction<void(bool)>& func = popupInfo->GetCallbackYesNoFunc();
				if (func)
				{
					func(false);
				}
			}

			_popupInfoList.Remove(popupInfo);
			++iter;
		}
	}
}

void UGsPopupSystemManager::OpenPopupSystemUI(const FText& InMessage)
{
	TWeakObjectPtr<UGsUIPopupSystem> popupSystemUI = GetPopupSystemUI();
	if (popupSystemUI.IsValid())
	{
		popupSystemUI->SetData(InMessage, [this]() {OnPopUpClose(true);});
		_popupSystemUI = popupSystemUI;
	}
}

void UGsPopupSystemManager::OpenPopupSystemYesNoUI(const FText& InMessage)
{
	TWeakObjectPtr<UGsUIPopupSystemYesNo> popupSystemUI = GetPopupSystemYesNo();
	if (popupSystemUI.IsValid())
	{
		popupSystemUI->SetData(InMessage, [this](bool bYes)
			{
				OnPopUpClose(bYes);
			});
		_popupSystemUI = popupSystemUI;
	}
}

void UGsPopupSystemManager::PopupSystemMsg(const FText& InMessage, TFunction<void()> InCloseCallback, 
													 EPopupType InPopupType, EPopupOpenType InPopupOpenType)
{
	switch (InPopupOpenType)
	{
	case EPopupOpenType::Serial:
		{
			if (IsVaildPopupType(InPopupType))
			{
				return;
			}

			_popupInfoList.Emplace(
				MakeShareable(new FPopupInfo(InPopupType, EPopupButtonType::Yes, InMessage, InCloseCallback)));
		}
		break;
	case EPopupOpenType::Parallel:
		{
			TWeakObjectPtr<class UGsUIPopupSystem> popupSystemUI = GetPopupSystemUI();
			if (popupSystemUI.IsValid())
			{
				popupSystemUI->SetData(InMessage, InCloseCallback);
			}
		}
		break;
	default:
		break;
	}

	if (1 == _popupInfoList.Num())
	{
		OpenPopupSystemUI(InMessage);
	}
}

void UGsPopupSystemManager::PopupSystemYesNo(const FText& InMessage, TFunction<void(bool)> InCloseCallback,
													EPopupType InPopupType, EPopupOpenType InPopupOpenType)
{
	switch (InPopupOpenType)
	{
	case EPopupOpenType::Serial:
	{
		if (IsVaildPopupType(InPopupType))
		{
			return;
		}

		_popupInfoList.Emplace(
			MakeShareable(new FPopupInfo(InPopupType, EPopupButtonType::YesNo, InMessage, nullptr, InCloseCallback)));
	}
	break;
	case EPopupOpenType::Parallel:
	{
		TWeakObjectPtr<class UGsUIPopupSystemYesNo> popupSystemUI = GetPopupSystemYesNo();
		if (popupSystemUI.IsValid())
		{
			popupSystemUI->SetData(InMessage, InCloseCallback);
		}
	}
	break;
	default:
		break;
	}

	if (_popupSystemUI.IsValid())
	{
		if (!_popupSystemUI->IsVisible())
		{
			OpenPopupSystemYesNoUI(InMessage);
		}
	}
	else
	{
		OpenPopupSystemYesNoUI(InMessage);
	}
}

bool UGsPopupSystemManager::IsVaildPopupType(EPopupType InPopupType)
{
	bool isfind = _popupInfoList.ContainsByPredicate([InPopupType](TSharedPtr<FPopupInfo> el)
		{
			if (!el.IsValid())
				return false;

			EPopupType popupType = el->GetPopupType();
			if (popupType == EPopupType::None)
				return false;

			return popupType == InPopupType;
		});

	return isfind;
}

void UGsPopupSystemManager::OnPopUpClose(bool InIsOk)
{
	if (!_popupInfoList.IsValidIndex(0))
	{
		return;
	}

	TSharedPtr<FPopupInfo> popupInfo = _popupInfoList[0];
	if (nullptr == popupInfo || !popupInfo.IsValid())
	{ 
		return;
	}

	_popupInfoList.RemoveAt(0);
	if (_popupInfoList.IsValidIndex(0))
	{
		TSharedPtr<FPopupInfo> newPopupInfo = _popupInfoList[0];
		if (newPopupInfo->GetButtonType() == EPopupButtonType::Yes)
		{
			OpenPopupSystemUI(newPopupInfo->GetMessage());
		}
		else
		{
			OpenPopupSystemYesNoUI(newPopupInfo->GetMessage());
		}
	}

	if (popupInfo->GetButtonType() == EPopupButtonType::Yes)
	{
		const TFunction<void()>& func = popupInfo->GetCallbackYesFunc();
		if (func)
		{
			func();
		}
	}
	else
	{
		const TFunction<void(bool)>& func = popupInfo->GetCallbackYesNoFunc();
		if (func)
		{
			func(InIsOk);
		}
	}	
}

TWeakObjectPtr<UGsUIPopupSystem> UGsPopupSystemManager::GetPopupSystemUI()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		return Cast<UGsUIPopupSystem>(uiMgr->OpenAndGetWidgetByPath(_popupOkKey, _popupOkPath));
	}

	return nullptr;
}

TWeakObjectPtr<UGsUIPopupSystemYesNo> UGsPopupSystemManager::GetPopupSystemYesNo()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		return Cast<UGsUIPopupSystemYesNo>(uiMgr->OpenAndGetWidgetByPath(_popupYesNoKey, _popupYesNoPath));
	}

	return nullptr;
}