// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowNetAddress.h"

#include "GsSchemaNetAddress.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"

#include "Components/WrapBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/UIContent/Window/ServerSelect/GsUIServerListSlot.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUIWindowNetAddress::NativeOnInitialized()
{
	// 서버 슬롯 동적생성 관리객체 생성
	_panelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelSlotHelper->Initialize(_entryWidgetClass, _panelServerlist);
	_panelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowNetAddress::OnRefreshEntry);

	if (nullptr != _btnConnect)
	{
		_btnConnect->OnClicked.AddDynamic(this, &UGsUIWindowNetAddress::OnClickConnect);
	}

	Super::NativeOnInitialized();
}

void UGsUIWindowNetAddress::BeginDestroy()
{
	if (_panelSlotHelper)
	{
		_panelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowNetAddress::OnRefreshEntry);
	}
	_panelSlotHelper = nullptr;

	Super::BeginDestroy();
}


void UGsUIWindowNetAddress::NativeConstruct()
{
	_serverIndex = 0;

	Super::NativeConstruct();
}

void UGsUIWindowNetAddress::NativeDestruct()
{	
	Super::NativeConstruct();
}

void UGsUIWindowNetAddress::SetData(/*const FText& InWorldName*/)
{
	_listServer.Empty();

#ifdef LOBBY_FLOW_UI_HIVE_1ST
	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		//_listServer = lobbyDataMgr->GetSeverList();

		if (const UGsTable* table = FGsSchemaNetAddress::GetStaticTable())
		{
			TArray<const FGsSchemaNetAddress*> allRow;

			if (false == table->GetAllRows(allRow))
			{
				return;
			}

			for (const FGsSchemaNetAddress* data : allRow)
			{
				FString rowName = data->GetRowName().ToString();
				_listServer.Emplace(rowName);
			}
		}

		int8 listNum = _listServer.Num();
		if (0 >= listNum)
			return;	

		_panelSlotHelper->RefreshAll(listNum);

		_toggleGroup.Clear();
		_toggleGroup.AddToggleWidgetByParentPanel(_panelServerlist);
		_toggleGroup.SetSelectedIndex(0);
	}
#else
#if WITH_EDITOR
	if (const UGsTable* table = FGsSchemaNetAddress::GetStaticTable())
	{
		TArray<const FGsSchemaNetAddress*> allRow;

		if (false == table->GetAllRows(allRow))
		{
			return;
		}

		for (const FGsSchemaNetAddress* data : allRow)
		{
			FString rowName = data->GetRowName().ToString();
			_listServer.Emplace(rowName);
		}
	}
#else
	_listServer = GDivergence()->GetAuthAgentNameList();		
#endif

	int8 listNum = _listServer.Num();
	if (0 >= listNum)
		return;

	_panelSlotHelper->RefreshAll(listNum);

	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelServerlist);
	_toggleGroup.SetSelectedIndex(0);
#endif
}

void UGsUIWindowNetAddress::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	// TODO: 각 슬롯별 정보 처리
	int serverNum = _listServer.Num();
	if (0 < serverNum &&
		InIndex <= serverNum)
	{
		// TODO: 각 슬롯별 정보 처리
		UGsUIServerListSlot* entry = Cast<UGsUIServerListSlot>(InEntry);
		if (entry)
		{
			const FString& serverName = _listServer[InIndex];
			(0 == InIndex) ? entry->SetData(InIndex, serverName, true) : entry->SetData(InIndex, serverName);
			entry->OnClickSlot.BindUObject(this, &UGsUIWindowNetAddress::OnClickSlot);
		}
	}
}

void UGsUIWindowNetAddress::OnClickSlot(int32 InIndex)
{
	// TODO: 바뀐 인덱스에 대한 처리
	GSLOG(Log, TEXT("[ServerSelect] list Index: %d"), InIndex);

	_serverIndex = InIndex;
	_toggleGroup.SetSelectedIndex(InIndex);
}

void UGsUIWindowNetAddress::OnClickConnect()
{
	int NumServerCnt = _listServer.Num();
	// bak1210 : _serverIndex는 음수값이 없어 초기화 될떄 0으로 될 수 있고, 배열크기역시 0일수 있어 0일때 접근할수 있는 오류 수정
	if ((NumServerCnt != 0) && (NumServerCnt >= _serverIndex))
	{
		OnClickServer.ExecuteIfBound(_serverIndex);
	}
}

bool UGsUIWindowNetAddress::OnBack()
{	
	FGsUIHelper::PopupQuitGame();

	// 게임 나가기 팝업창이 아닌 종료창을 띄울 것이므로 Super::OnBack를 타지 않음
	return true;	
}
