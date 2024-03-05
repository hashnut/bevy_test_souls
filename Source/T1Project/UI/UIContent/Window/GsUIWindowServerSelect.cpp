// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowServerSelect.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/WrapBox.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/ServerSelect/GsUIServerListSlot.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Option/GsGameUserSettings.h"


void UGsUIWindowServerSelect::NativeOnInitialized()
{
	// 서버 슬롯 동적생성 관리객체 생성
	_panelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelSlotHelper->Initialize(_entryWidgetClass, _panelServerlist);
	_panelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowServerSelect::OnRefreshEntry);

	if (nullptr != _btnConnect)
	{
		_btnConnect->OnClicked.AddDynamic(this, &UGsUIWindowServerSelect::OnClickConnect);
	}

	Super::NativeOnInitialized();
}

void UGsUIWindowServerSelect::BeginDestroy()
{
	if (_panelSlotHelper)
	{
		_panelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowServerSelect::OnRefreshEntry);
	}
	_panelSlotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIWindowServerSelect::SetData(/*const FText& InWorldName*/)
{
	//_listServer.Empty();

	//if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	//{
	//	_listServer = lobbyDataMgr->GetSeverList();

	//	int8 listNum = _listServer.Num();
	//	if (0 >= listNum)
	//		return;

	//	/*if (UGsOptionManager* optionManager = GOption())
	//	{
	//		if (UGsGameUserSettings* userSettings = optionManager->GetGameUserSettings())
	//		{
	//			const FString& serverName = userSettings->GetUserInfoSetting(EGsOptionUserInfo::LAST_ENTER_WORLD_NAME);
	//			if (!serverName.IsEmpty())
	//			{
	//				WorldId id = static_cast<WorldId>(FCString::Atoi(*serverName));
	//				
	//				for (int i = 0; i < listNum; ++i)
	//				{
	//					if (id == _listServer[i].mPlanetWorldId)
	//					{
	//						if (0 != i)
	//						{
	//							_listServer.Swap(i, 0);
	//						}
	//						break;
	//					}
	//				}										
	//			}
	//		}
	//	}*/

	//	_panelSlotHelper->RefreshAll(listNum);
	//			
	//	_toggleGroup.Clear();
	//	_toggleGroup.AddToggleWidgetByParentPanel(_panelServerlist);
	//	_toggleGroup.SetSelectedIndex(0);
	//}
}

void UGsUIWindowServerSelect::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	//// TODO: 각 슬롯별 정보 처리
	//if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	//{
	//	int serverNum = _listServer.Num();
	//	if (0 < serverNum &&
	//		InIndex <= serverNum)
	//	{
	//		// TODO: 각 슬롯별 정보 처리
	//		UGsUIServerListSlot* entry = Cast<UGsUIServerListSlot>(InEntry);
	//		if (entry)
	//		{
	//			const FString& serverName = _listServer[InIndex].mName;
	//			(0 == InIndex) ? entry->SetData(InIndex, serverName, true) : entry->SetData(InIndex, serverName);
	//			entry->OnClickSlot.BindUObject(this, &UGsUIWindowServerSelect::OnClickSlot);
	//		}
	//	}
	//}
}

void UGsUIWindowServerSelect::OnClickSlot(int32 InIndex)
{
	// TODO: 바뀐 인덱스에 대한 처리
	GSLOG(Log, TEXT("[ServerSelect] list Index: %d"), InIndex);

	_serverIndex = InIndex;
	_toggleGroup.SetSelectedIndex(InIndex);
}

void UGsUIWindowServerSelect::OnClickConnect()
{
	if (_listServer.Num() >= _serverIndex)
	{
		//FGsNetSendService::SendAuthReqSelectServer(list[_serverIndex].mPlanetWorldId);
		OnClickServer.ExecuteIfBound(_listServer[_serverIndex].mPlanetWorldId);
	}
}

bool UGsUIWindowServerSelect::OnBack()
{
	return false;
}
