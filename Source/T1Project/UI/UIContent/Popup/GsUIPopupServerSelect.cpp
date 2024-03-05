// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupServerSelect.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "Option/GsGameUserSettings.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/WrapBox.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/ServerSelect/GsUIServerListSlot.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "InterServer/World/GsSchemaWorldData.h"


void UGsUIPopupServerSelect::NativeOnInitialized()
{
	// 서버 슬롯 동적생성 관리객체 생성
	_panelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelSlotHelper->Initialize(_entryWidgetClass, _panelServerlist);
	_panelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupServerSelect::OnRefreshEntry);

	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupServerSelect::OnClickClose);
	}

	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIPopupServerSelect::OnSelectTab);
	_toggleGroupTab.AddToggleWidgetByParentPanel(_panelTabListRoot);

	Super::NativeOnInitialized();
}


void UGsUIPopupServerSelect::NativeConstruct()
{
	Super::NativeConstruct();

	// refreshall 하여 widget을 child로 생성하는 로직이므로 Super::NativeConstruct 이후에 해준다 - parent construct 처리 필요
	// https://com2us-raon.sentry.io/issues/4284397081 가능성
	_toggleGroupTab.SetSelectedIndex(0, true);
}

void UGsUIPopupServerSelect::NativeDestruct()
{
	//_serverIndex = 0;

	Super::NativeDestruct();
}

void UGsUIPopupServerSelect::BeginDestroy()
{
	if (_panelSlotHelper)
	{
		_panelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupServerSelect::OnRefreshEntry);
	}
	_panelSlotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupServerSelect::SetData(int32 InIndex)
{
	FGsNetManager* netManager = GNet();
	if (nullptr == netManager)
	{
		return;
	}

	const TArray<ServerElem>& serverList = netManager->GetSeverList();
	int8 listNum = serverList.Num();
	if (listNum <= 0)
	{
		return;
	}

	uint16 selectWorldId = netManager->GetPlanetWorldId();

	_listServer.Empty();
	// 0 추천 서버
	// 1 전체 서버
	// 2 캐릭터 보유 서버
	switch (static_cast<ServerListType>(InIndex))
	{
	case ServerListType::Server_Recommended:
		for (size_t i = 0; i < listNum; ++i)
		{
			if (true == serverList[i].mRecommended)
			{
				_listServer.Emplace(serverList[i]);

				if (serverList[i].mPlanetWorldId == selectWorldId)
				{
					_serverIndex = i;
				}
			}
		}
		break;
	case ServerListType::Server_All:
		for (size_t i = 0; i < listNum; ++i)
		{			
			_listServer.Emplace(serverList[i]);

			if (serverList[i].mPlanetWorldId == selectWorldId)
			{
				_serverIndex = i;
			}			
		}
		break;
	case ServerListType::Server_My:
		for (size_t i = 0; i < listNum; ++i)
		{
			if (0 < serverList[i].mUserCount)
			{
				_listServer.Emplace(serverList[i]);

				if (serverList[i].mPlanetWorldId == selectWorldId)
				{
					_serverIndex = i;
				}
			}
		}
		break;
	default:
		break;
	}

	int8 viewListNum = _listServer.Num();
	_panelSlotHelper->RefreshAll(viewListNum);

	if (0 < viewListNum)
	{
		_toggleGroup.Clear();
		_toggleGroup.AddToggleWidgetByParentPanel(_panelServerlist);
		_toggleGroup.SetSelectedIndex(_serverIndex);
	}
}

void UGsUIPopupServerSelect::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	// TODO: 각 슬롯별 정보 처리
	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		int serverNum = _listServer.Num();
		if (0 < serverNum &&
			InIndex <= serverNum)
		{
			// TODO: 각 슬롯별 정보 처리
			UGsUIServerListSlot* entry = Cast<UGsUIServerListSlot>(InEntry);
			if (entry)
			{				
				uint16 planetWorlId = _listServer[InIndex].mPlanetWorldId;

				FString serverName;
				GNet()->GetPlanetWorldIdName(planetWorlId, serverName);

				(0 == InIndex) ? entry->SetData(InIndex, serverName, _listServer[InIndex].mUserCount, 
					_listServer[InIndex].mCongestion, _listServer[InIndex].mCharCreatable, 
					_listServer[InIndex].mRecommended, _listServer[InIndex].mNewOpened, true) : entry->SetData(InIndex, serverName, 
						_listServer[InIndex].mUserCount,
						_listServer[InIndex].mCongestion, _listServer[InIndex].mCharCreatable,
						_listServer[InIndex].mRecommended, _listServer[InIndex].mNewOpened);
				entry->OnClickSlot.BindUObject(this, &UGsUIPopupServerSelect::OnClickSlot);
			}
		}
	}
}

void UGsUIPopupServerSelect::OnClickSlot(int32 InIndex)
{
	// TODO: 바뀐 인덱스에 대한 처리
	GSLOG(Log, TEXT("[ServerSelect] list Index: %d"), InIndex);

	_serverIndex = InIndex;
	_toggleGroup.SetSelectedIndex(InIndex);
}

void UGsUIPopupServerSelect::OnClickClose()
{
	/*if (_slotCallback)
	{
		_slotCallback(_serverIndex);
	}*/

	if (_serverIndex < _listServer.Num())
	{
		OnClickServer.ExecuteIfBound(_listServer[_serverIndex].mPlanetWorldId);
	}	

	Close();
}

void UGsUIPopupServerSelect::OnSelectTab(int32 InIndex)
{
	SetData(InIndex);
}