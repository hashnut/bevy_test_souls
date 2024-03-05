// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateDungeon.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Dungeon/GsDungeonData.h"
#include "Dungeon/GsPartyDungeonState.h"
#include "Dungeon/GsDungeonGroupBase.h"
#include "Dungeon/GsDungeonCommonData.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"

#include "UI/UIContent/Window/GsUIWindowDungeon.h"
#include "UI/UIContent/Window/GsUIWindowDungeonPlayResult.h"

#include "Message/GsMessageUI.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "../Net/GsNetSendServiceWorld.h"


FGsGameStateDungeon::FGsGameStateDungeon() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsDungeon)
{
	_stateHandler.Initialize();
}

FGsGameStateDungeon::~FGsGameStateDungeon()
{
	_stateHandler.Finalize();
}

void FGsGameStateDungeon::Enter()
{
	FGsContentsGameBase::Enter();	

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowDungeon"));
		if (widget.IsValid())
		{
			_windowDungeonUI = Cast<UGsUIWindowDungeon>(widget);
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);

	// 메세지 등록
	RegisterMessages();
	// 서버로 목록 요청
	FGsNetSendServiceWorld::SendReqDungeonGroupList();
}

void FGsGameStateDungeon::Exit()
{	
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_windowDungeonUI = nullptr;
	_dungeonId = 0;
	// 메세지 해지
	UnregisterMessages();

	FGsContentsGameBase::Exit();
}


void FGsGameStateDungeon::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	
	_listDungeonDelegate.Emplace(msg->GetDungeon().AddRaw(MessageContentDungeon::INVALIDATE_DATA,
		this, &FGsGameStateDungeon::OnInvalidate));

	_listDungeonDelegate.Emplace(msg->GetDungeon().AddRaw(MessageContentDungeon::WINDOW_MAP_ID,
		this, &FGsGameStateDungeon::OnShotcutDungeonMapId));

	_listDungeonDelegate.Emplace(msg->GetDungeon().AddRaw(MessageContentDungeon::SHOTCUT_PARTY_DUNGEON,
		this, &FGsGameStateDungeon::OnShotcutPartyDungeon));

	_listDungeonDelegate.Emplace(msg->GetDungeon().AddRaw(MessageContentDungeon::PARTY_DUNGEON_PARTYLIST,
		this, &FGsGameStateDungeon::OnInvalidatePartyInfoSet));
	
	_listDungeonDelegate.Emplace(msg->GetDungeon().AddRaw(MessageContentDungeon::CLOSE_WINDOW,
		this, &FGsGameStateDungeon::OnForceWindowClose));

	_listDungeonDelegate.Emplace(msg->GetDungeon().AddRaw(MessageContentDungeon::RESET_TIME,
		this, &FGsGameStateDungeon::OnCheckActivateWindow));

	_battleArenaDelegate = msg->GetBattleArena().AddRaw(MessageContentBattleArena::UPDATE_STATE_INFO, this, &FGsGameStateDungeon::OnInvalidateBattleArena);
}

void FGsGameStateDungeon::UnregisterMessages()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		for (auto& item : _listDungeonDelegate)
		{
			msg->GetDungeon().Remove(item);
		}
		_listDungeonDelegate.Empty();

		msg->GetBattleArena().Remove(_battleArenaDelegate);
	}
}

void FGsGameStateDungeon::OnInvalidate(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = (InParam) ? InParam->Cast<const FGsPrimitiveInt32>() : nullptr;

	if (_windowDungeonUI.IsValid())
	{	// 2022/06/09 PKT - 파티 던전 화면 갱신
		DungeonGroupId invalidGroupId = (nullptr == param) ? INVALID_DUNGEON_GROUP_ID : param->_data;
		_windowDungeonUI->Invalidate(invalidGroupId);
	}
}

void FGsGameStateDungeon::OnShotcutDungeonMapId(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>();
	if (param)
	{		
		if (_windowDungeonUI.Get()->IsActive())
		{
			_windowDungeonUI.Get()->GoSelectedDungeonGroup(param->_data);
		}
	}
}

void FGsGameStateDungeon::OnShotcutPartyDungeon(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}
	const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>();

	FGsDungeonManager* dungeonManager = GSDungeon();
	if (nullptr == dungeonManager)
	{
		GSLOG(Error, TEXT("nullptr == dungeonManager"));
		return;
	}

	const FGsDungeonGroupBase* dungeonGroup = dungeonManager->FindGroupDataById(param->_first);
	const FGsSchemaDungeonGroupData* schemaData = (nullptr != dungeonGroup) ? dungeonGroup->GetSchemaData() : nullptr;
	if (nullptr == schemaData || DungeonCategory::PARTY != schemaData->dungeonCategory)
	{
		GSLOG(Error, TEXT("Invalidate DungeonGroupId [%d]"), param->_first);
		return;
	}

	for (const auto& item : schemaData->dungeonList)
	{
		const FGsSchemaDungeonData* dungeonData = item.GetRow();

		if (dungeonData && param->_second == StaticCast<int32>(dungeonData->levelOfDifficulty))
		{
			const FGsSchemaMapData* mapData = dungeonData->mapId.GetRow();
			if (mapData)
			{
				if (_windowDungeonUI.Get()->IsActive())
				{
					_windowDungeonUI.Get()->GoSelectedDungeonGroup(mapData->id);
				}

				break;
			}
		}
	}
}

void FGsGameStateDungeon::OnInvalidatePartyInfoSet(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamPartyDungeonMemberList* param = InParam->Cast<const FGsUIMsgParamPartyDungeonMemberList>();
	if (_windowDungeonUI.IsValid() && param)
	{	// 2022/06/02 PKT - 파티 던전 파티 리스트 갱신
		_windowDungeonUI->InvalidatePartyList(param->_dataList);
	}
}

void FGsGameStateDungeon::OnForceWindowClose(const IGsMessageParam* InParam)
{
	if (false == _windowDungeonUI.IsValid())
	{
		return;
	}

	if (_windowDungeonUI.Get()->IsActive())
	{
		_windowDungeonUI.Get()->Close();
	}
}

void FGsGameStateDungeon::OnCheckActivateWindow(const IGsMessageParam* InParam)
{
	if (_windowDungeonUI.IsValid() && _windowDungeonUI.Get()->IsActive())
	{	// 2022/10/26 PKT - Window가 열려 있으면 갱신을 요청한다.
		FGsNetSendServiceWorld::SendReqDungeonGroupList();
	}
}

void FGsGameStateDungeon::OnInvalidateBattleArena(const IGsMessageParam* InParam)
{
	if (_windowDungeonUI.IsValid() && _windowDungeonUI.Get()->IsActive())
	{	// 2022/10/26 PKT - Window가 열려 있으면 갱신을 요청한다.
		_windowDungeonUI.Get()->InvalidateBattleArena();
	}
}

FGsDungeonStateHandler* FGsGameStateDungeon::StateHandler()
{
	return &_stateHandler;
}