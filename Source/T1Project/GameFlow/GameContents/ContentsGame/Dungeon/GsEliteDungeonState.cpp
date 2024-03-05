

#include "GsEliteDungeonState.h"

#include "../Dungeon/GsDungeonCommonEnum.h"

#include "../Dungeon/GsDungeonHelper.h"
#include "../Dungeon/GsDungeonData.h"
#include "../Dungeon/GsDungeonGroupBase.h"

#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGlobal/GsQuestManager.h"
#include "../Management/ScopeGame/GsDungeonManager.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"

#include "../Message/GsMessageContentHud.h"
#include "../Message/GsMessageGameObject.h"
#include "../Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"


#include "../GameObject/Define/GsGameObjectDefine.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "../GameObject/ObjectClass/GsGameObjectPropInteractionPotal.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNPCGateKeeper.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "Shared/Client/SharedEnums/SharedPropEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UTIL/GsTableUtil.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "T1Project.h"


bool FGsEliteDungeonState::Enter(MapId InMapId)
{
	if (false == Super::Enter(InMapId))
	{
		return false;
	}

	// 기존 FGsEliteDungeonState Enter시는 서버 패킷 받는 시점이라
	// GsGameFlowGame::OnChangeLevel 보다 먼저 일어남
	// FGsGameFlowGame::OnEnterDungeon 시점에 맵타입이 ELITE_DUNGEON_1 일때만 별도 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-8568

	//if (FGsMessageHolder* messageMgr = GMessage())
	//{
	//	FGsUIMsgParamBool param(true);
	//	messageMgr->GetGameObject().SendMessage(MessageGameObject::MINIMAP_ELITE_DUNGEON_ACTIVE, &param);
	//}

	return true;
}

void FGsEliteDungeonState::Exit()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		FGsUIMsgParamBool param(false);
		messageMgr->GetGameObject().SendMessage(MessageGameObject::MINIMAP_ELITE_DUNGEON_ACTIVE, &param);
	}

	Super::Exit();
}

void FGsEliteDungeonState::HUDTimeUpdate(DungeonGroupId InGoupId /* = INVALID_DUNGEON_GROUP_ID */)
{
	const DungeonGroupId id = GetGroupId();
	const DungeonCategory category = GetCategory();

	if (INVALID_DUNGEON_GROUP_ID != InGoupId && id != InGoupId)
	{	// 2023/12/11 PKT - 
		return;
	}

	const FGsDungeonGroupBase* dungeonGroup = GSDungeon()->FindGroupDataById(id);
	if (dungeonGroup && category == dungeonGroup->GetCategory())
	{
		const FGsDungeonGroupNormal* dungoneGroupData = StaticCast<const FGsDungeonGroupNormal*>(dungeonGroup);
		if (dungoneGroupData)
		{
			SendMessageHUDTimeUpdate(dungoneGroupData->GetTime(), dungoneGroupData->IsTimeLimit(), InIsLobbyFloor());
		}
	}
}

FGsDungeonStateBase::HUDTimeMode FGsEliteDungeonState::GetHUDTimeMode() const
{
	HUDTimeMode mode = FGsDungeonStateBase::GetHUDTimeMode();
	if (HUDTimeMode::Play == mode)
	{	// 2022/11/02 PKT - lobby에서는 시간이 가지 않는다.
		mode = InIsLobbyFloor() ? HUDTimeMode::Pause : HUDTimeMode::Play;
	}

	return mode;
}

bool FGsEliteDungeonState::IsExitNotifyMessage(FText& InMessage1, FText& InMessage2)
{
	InMessage1 = InMessage2 = FText::GetEmpty();

	FText format;
	if (InIsLobbyFloor())
	{	// 2022/07/28 PKT - {0} \n 나가시겠습니까?
		FGsDungeonHelper::FindText(TEXT("Popup_Common_Leave"), format);
	}
	else
	{	// 2022/07/28 PKT - {0}\n(로,으로) 나가시겠습니까?
		FGsDungeonHelper::FindText(TEXT("Popup_Elite_Leave_NFloor"), format);
	}

	const MapId currentMapId = GetMapId();

	const FGsDungeonGroupBase* dungeonGroupData = GSDungeon()->FindGroupDataByMapId(currentMapId);
	
	const int32 LOBBY_INDEX = 0;
	const FGsDungeonData* dungeonData = (dungeonGroupData) ? dungeonGroupData->FindDungeonByIndex(LOBBY_INDEX) : nullptr;
	if (dungeonData)
	{
		FText name;
		if (FGsDungeonHelper::DungeonNameByMapData(dungeonGroupData->GetSchemaData(), dungeonData->GetSchema(), name))
		{
			// 2022/07/28 PKT - Text 조합
			InMessage1 = FText::Format(format, name);
		}
	}

	return true;
}

bool FGsEliteDungeonState::InIsLobbyFloor() const
{
	if (EDungeonProgressState::EInDungeon != _progressState)
	{
		return false;
	}

	const MapId currentMapId = GetMapId();

	const FGsDungeonGroupBase* findGroupData = GSDungeon()->FindGroupDataByMapId(currentMapId);
	const FGsDungeonData* findData = GSDungeon()->FindDungeonDataByMapId(currentMapId);
	if (findGroupData && findData)
	{
		static const int32 LOBBY_FLOOR_INDEX = 0;

		const int32 findIndex = findGroupData->FindIndexByData(findData);
		if (LOBBY_FLOOR_INDEX == findIndex)
		{
			return true;
		}
	}

	return false;
}

// 2022/11/01 PKT - Elite Dungeon Keeper NPC
void FGsEliteDungeonState::InteractionNpcKeeperContents(int32 InSchemaTableId)
{
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(InSchemaTableId);
	if (nullptr == npcData)
	{
		GSLOG(Warning, TEXT("[Quest]FGsSchemaNpcData::FindRowById is EmptyData - InSchemaTableId : %d"), InSchemaTableId);
		return;
	}

	if (NpcFunctionType::GATEKEEPER != npcData->npcFunctionType)
	{	// 2022/07/25 PKT - NPC 키퍼 타입 검사
		return;
	}

	const FGsSchemaNPCGateKeeper* gateKeeper = UGsTableUtil::FindRowById<UGsTableNPCGateKeeper, FGsSchemaNPCGateKeeper>(InSchemaTableId);
	if (nullptr == gateKeeper)
	{
		GSLOG(Warning, TEXT("[Quest]FGsSchemaNPCGateKeeper::FindRowById is EmptyData - InSchemaTableId : %d"), InSchemaTableId);
		return;
	}

	if (GateKeeperContens::DUNGEON != gateKeeper->contents)
	{	// 2022/07/25 PKT - 던전 컨텐츠가 아니면 여기서 처리 안함.
		return;
	}

	const FGsSchemaMapData* mapData = gateKeeper->mapId.GetRow();
	if (nullptr == mapData)
	{
		GSLOG(Error, TEXT("nullptr == mapData"));
		return;
	}

	// 2022/07/25 PKT - 다른 층으로 입장 시도.
	GSDungeon()->TryToEnterDungeon(mapData->id);
}

// 2022/11/01 PKT - Elite Dungeon Prop Potal
void FGsEliteDungeonState::InteractionPropPortalStart(int32 InSchemaTableId, int64 InGameId)
{
	//const FGsPropInteractionMessageParamContents* interactionParam = InData->Cast<const FGsPropInteractionMessageParamContents>();
	const FGsSchemaPropData* schemaData = UGsTableUtil::FindRowById<UGsTablePropData, FGsSchemaPropData>(InSchemaTableId);
	if (nullptr == schemaData)
	{
		GSLOG(Error, TEXT("nullptr == schemaData : [%d]"), InSchemaTableId);
		return;
	}

	if (PropType::ELITE_DUNGEON_PORTAL_UP != schemaData->propType && PropType::ELITE_DUNGEON_PORTAL_HIDDEN != schemaData->propType)
	{	// 2023/7/27 PKT - 정예던전에는 다른층으로 가는 포털과 히든 영역으로 이동하는 포털 2가지가 존재
		return;
	}

	UGsGameObjectBase* object = GSGameObject()->FindObject(EGsGameObjectType::PropInteraction, InGameId);
	if (nullptr == object || false == object->IsA<UGsGameObjectPropInteractionPotal>())
	{
		GSLOG(Error, TEXT("nullptr == object || false == object->IsA<UGsGameObjectPropInteractionPotal>() : [%lu]"), InGameId);
		return;
	}

	const UGsGameObjectPropInteractionPotal* propPotal = Cast<UGsGameObjectPropInteractionPotal>(object);
	if (nullptr == propPotal)
	{
		GSLOG(Error, TEXT("nullptr == propPotal"));
		return;
	}

	FText message = FText::GetEmpty();		
	if (PropType::ELITE_DUNGEON_PORTAL_UP == schemaData->propType)
	{
		const FGsSchemaMapData* mapData = propPotal->GetDestinationMapData();
		if (mapData)
		{
			FText format;
			FGsDungeonHelper::FindText(TEXT("Popup_Elite_Enter_UsePortal"), format);
			message = FText::Format(format, { mapData->MapStringId, propPotal->GetUseCount(), schemaData->multipleInteractionCountMax });
		}			
	}
	else if (PropType::ELITE_DUNGEON_PORTAL_HIDDEN == schemaData->propType)
	{
		FGsDungeonHelper::FindText(TEXT("Popup_Elite_HiddenPortal"), message);
	}
	

	FGsInteractionMessageParam param(InGameId, EGsGameObjectType::PropInteraction);
	FGsUIHelper::PopupYesNo(message, [param](bool isOK)
		{
			if (isOK)
			{
				GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_BUTTON_CLICK, &param);
			}
		}
	);
}

void FGsEliteDungeonState::InteractionPropPortalEnd(int64 InGameId)
{
	if (0 != InGameId)
	{
		FGsNetSendServiceWorld::SendInteractPropEnd(InGameId);
	}	
}