// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameDungeon.h"
#include "GsStageGameBase.h"

#include "Dungeon/GsDungeonData.h"
#include "Dungeon/GsDungeonGroupBase.h"

#include "../Dungeon/GsDungeonHelper.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "GameFlow/GameContents/ContentsGame/Dungeon/GsPartyDungeonState.h"

#include "../GameObject/Define/GsGameObjectDefine.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "../GameObject/ObjectClass/GsGameObjectPropInteractionPotal.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/HUD/GsUIHUDDungeonTime.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"

#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaCoopPartyDungeonConfig.h"

#include "../UTIL/GsTimeSyncUtil.h"
#include "../UI/UILib/Define/GsUIDefine.h"
#include "../UI/UIContent/Window/GsUIWindowDungeonPlayResult.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Map/GsSchemaMapData.h"



FGsStageGameDungeon::FGsStageGameDungeon() : FGsStageGameBase(FGsStageMode::Game::DUNGEON)
{
}

FGsStageGameDungeon::~FGsStageGameDungeon()
{
}

void FGsStageGameDungeon::Enter()
{
	FGsStageGameBase::Enter();

	GUI()->SetHUDMode(EGsUIHUDMode::NORMAL);


}

void FGsStageGameDungeon::Exit()
{
	GUI()->SetHUDMode(EGsUIHUDMode::NORMAL);

	FGsStageGameBase::Exit();

}


void FGsStageGameDungeon::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);

	/**
	 * None
	 */
}

// 2022/07/27 PKT - 던전 플레이가 시작 되었음.(공통 - 파티 던전 제외)
void FGsStageGameDungeon::RecvDungeonEnter(struct PD::SC::PKT_SC_DUNGEON_ENTER_READ& InPacket)
{
	int32 baseTime = InPacket.MyBaseTime();
	int32 addTime = InPacket.MyAddTime();

	// 2022/08/03 PKT - 시간 값 다시 세팅(동기화 맞추기 위해..)
	GSDungeon()->SyncDungeonGroupTime(InPacket.DungeonGroupId(), baseTime, addTime);
	
	{
		FGsPrimitiveInt32 param(InPacket.MapId());
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::ENTER, &param);
	}
	
	{
		FGsPrimitiveInt32 param(InPacket.DungeonGroupId());
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::UPDATE_TIME, &param);
	}	
}

// 2022/07/27 PKT - 던전 플레이가 끝났음.(들어올때와는 다르게 파티던전에서도 나갔을 경우 패킷이 날라옴)
void FGsStageGameDungeon::RecvDungeonExit(struct PD::SC::PKT_SC_DUNGEON_LEAVE_READ& InPacket)
{
	int32 baseTime = InPacket.MyBaseTime();
	int32 addTime = InPacket.MyAddTime();

	// 2022/08/03 PKT - 시간 값 다시 세팅(이용 시간 차감)
	GSDungeon()->SyncDungeonGroupTime(InPacket.DungeonGroupId(), baseTime, addTime);

	GMessage()->GetDungeon().SendMessage(MessageContentDungeon::EXIT, nullptr);
}

void FGsStageGameDungeon::RecvDungeonInfo(struct PD::SC::PKT_SC_DUNGEON_INFO_READ& InPacket)
{
	// 2022/10/11 PKT - 플레이어 사망 등에 의해 던전 시간 값을 동기화 시켜야 할때 패킷이 옴.
	int32 baseTime = InPacket.MyBaseTime();
	int32 addTime = InPacket.MyAddTime();
	GSDungeon()->SyncDungeonGroupTime(InPacket.DungeonGroupId(), baseTime, addTime);

	FGsPrimitiveInt32 param(InPacket.DungeonGroupId());
	GMessage()->GetDungeon().SendMessage(MessageContentDungeon::UPDATE_TIME, &param);
}

void FGsStageGameDungeon::PartyDungeonProgressState(PD::SC::PKT_SC_PARTY_DUNGEON_INFO_READ& InPacket)
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}
	
	const FGsDungeonStateHandler* dungeonStateHandler = gameStateDungeon->StateHandler();
	if (nullptr == dungeonStateHandler)
	{
		GSLOG(Error, TEXT("nullptr == dungeonStateHandler"));
		return;
	}

	const FGsPartyDungeonState* partyDungeonState = StaticCast<const FGsPartyDungeonState*>(dungeonStateHandler->GetState(DungeonCategory::PARTY));
	if (nullptr == partyDungeonState)
	{
		GSLOG(Error, TEXT("nullptr == partyDungeonState"));
		return;
	}

	if (EDungeonProgressState::EInDungeon != partyDungeonState->GetProgressState())
	{
		// 2023/2/16 PKT - 아직 던전 입장 처리가 안된 것.
		FGsPrimitiveInt32 param(InPacket.MapId());
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::ENTER, &param);
	}

	switch (InPacket.Status())
	{
	case PartyDungeonStepStatus::WAITING:	// 2022/06/10 PKT - 던전 입장 전 파티 입장 대기 상태
	// 2022/06/10 PKT - 이때는 아무것도 안함. 아직 던전 입장 전 상태
	break;
	case PartyDungeonStepStatus::LOADING:
		break;
	case PartyDungeonStepStatus::COUNTDOWN:
	{
		FGsPrimitiveUInt64 param(InPacket.DungeonStartTime());
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::START_PARTY_DUNGEON_COUNT, &param);
	}	break;
	case PartyDungeonStepStatus::PLAYING:
	{	
		const FGsUIMsgParamDungeonTimeEX param(InPacket.DungeonStartTime(), InPacket.DungeonEndTime());
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::START_PARTY_DUNGEON_PLAY, &param);
	}break;
	case PartyDungeonStepStatus::JUST_BEFORE_END:	// 2022/06/10 PKT - 종료 직전
	{
		// 2022/06/10 PKT - 제한 시간 {0}초 남았습니다."
		FText message;
		FGsDungeonHelper::FindText(TEXT("Notice_Warning_TimeLeft"), message);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);

		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::CLOSE_IMMINENT_PARTY_DUNGEON_PLAY, nullptr);
	}break;
	case PartyDungeonStepStatus::END:		// 2022/06/10 PKT - 끝
		break;
	}
}

void FGsStageGameDungeon::DungeonResultNotify(PD::SC::PKT_SC_PARTY_DUNGEON_END_READ& InPacket)
{
	// 2023/3/3 PKT - 현재는 파티 던전에서만 활용..
	if (MapType::PARTY_DUNGEON != GLevel()->GetMapType())
	{
		GSLOG(Error, TEXT("MapType::PARTY_DUNGEON != GLevel()->GetMapType()"));
		return;
	}

	FGsUIMsgParamPartyDungeonResult message;
	message._result = InPacket.Success();
	message._playTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InPacket.DungeonEndTime() - InPacket.DungeonStartTime());
	message._basicCurrencyPair = InPacket.BasicRewardCurrency();
	message._personCurrencyPair = InPacket.MainRewardCurrency();
	InPacket.MakeBasicRewardItemListVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(message._basicRewardList);
	InPacket.MakeMainRewardItemListVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(message._mainRewardList);

	const FGsSchemaMapData* schemaMapData = GLevel()->GetCurrentMapInfo();
	const FGsDungeonGroupBase* dungeonGroupData = GSDungeon()->FindGroupDataByMapId(schemaMapData->id);
	const FGsDungeonData* dungeonData = dungeonGroupData->FindDungeonByMapId(schemaMapData->id);
	
	DungeonGroupId groupId = dungeonGroupData->GetGroupId();
	DungeonDifficulty difficulty = dungeonData->GetSchema()->levelOfDifficulty;

	TWeakObjectPtr<UGsUIWindowDungeonPlayResult> widget 
		= Cast<UGsUIWindowDungeonPlayResult>(GUI()->OpenAndGetWidget(TEXT("WindowDungeonPlayResult")));
	if (widget.IsValid())
	{
		widget.Get()->SetData(groupId, difficulty, &message);
	}

	// 2023/3/16 PKT - 파티 던전에서 나갈 때 이벤트로 받은 메세지를 모두 닫는다.
	IGsMessageParam param;
	GMessage()->GetEvent().SendMessage(MessageContentEvent::ALL_CLEAR_EVENT_ACTION_IMMEDIATE, param);
}

void FGsStageGameDungeon::EliteDungeonPropPotalInfo(PD::SC::PKT_SC_ELITEDUNGEON_PORTAL_INFO_READ& InPacket)
{
	if (MapType::ELITE_DUNGEON_1 != GLevel()->GetMapType())
	{
		return;
	}

	/**
	 * 2022/10/11 PKT - Prop에 이동 할 목적지(MapData)를 넘겨 준다.
	 */
	const int64 gameId = InPacket.GameId();
	const MapId destMapId = InPacket.MapId();

	const FGsDungeonData* dungeonData = GSDungeon()->FindDungeonDataByMapId(destMapId);
	if (nullptr == dungeonData)
	{
		return;
	}
		
	UGsGameObjectBase* object = GSGameObject()->FindObject(EGsGameObjectType::PropInteraction, gameId);
	if (nullptr == object || false == object->IsA<UGsGameObjectPropInteractionPotal>())
	{
		GSLOG(Error, TEXT("nullptr == object || false == object->IsA<UGsGameObjectPropInteractionPotal>() : [%lu]"), gameId);
		return;
	}

	UGsGameObjectPropInteractionPotal* propPotal = Cast<UGsGameObjectPropInteractionPotal>(object);
	if (nullptr == propPotal)
	{
		GSLOG(Error, TEXT("nullptr == propPotal"));
		return;
	}

	if (const FGsSchemaMapData* destMapData = dungeonData->GetSchema()->mapId.GetRow())
	{	// 2022/10/11 PKT - 목적지 설정
		propPotal->SetDestinationMapData(destMapData);
	}
}