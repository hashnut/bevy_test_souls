#include "GsMinimapIconHandler.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "Map/Minimap/MapIcon/GsMinimapIconObject.h"
#include "Map/Minimap/MapIcon/GsMinimapIconLocal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconOther.h"
#include "Map/Minimap/MapIcon/GsMinimapIconNpc.h"
#include "Map/Minimap/MapIcon/GsMinimapIconParty.h"
#include "Map/Minimap/MapIcon/GsMinimapIconInvasionNPC.h"
#include "Map/Minimap/MapIcon/GsMinimapIconInvasionPortal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconAutoMovePath.h"
#include "Map/Minimap/SceneImage/GsMinimapSceneImage.h"
#include "Map/Minimap/GsMinimapFunc.h"
#include "Map/WorldMap/GsRegionInvadeIconInfo.h"
#include "Map/GsMapFunc.h"

#include "Map/Bookmark/GsBookmarkData.h"
#include "Map/WorldMap/GsSpaceCrackPortalIconInfo.h"

#include "Quest/Management/GsQuestTargetManagement.h"

#include "SpaceCrack/GsSpaceCrackData.h"

#include "Party/GsPartyMemberInfo.h"

#include "DataSchema/icon/GsSchemaMinimapIconData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIContent/HUD/Minimap/GsUIIconMinimap.h"
#include "UI/UIContent/Popup/GsMinimapIconDetectionTarget.h"

#include "Level/GsLevelDefine.h"

#include "Option/GsServerOption.h"

#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "T1Project.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Quest/GsQuestData.h"


UGsMinimapIconHandler::~UGsMinimapIconHandler()
{
}

// 초기화
void UGsMinimapIconHandler::Initialize()
{
	InitializeMessage();
}
// 정리
void UGsMinimapIconHandler::Finalize()
{
	ClearAllIcon();

	_iconLocal = nullptr;

	FinalizeMessage();
}

void UGsMinimapIconHandler::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();
	MGameObject& msgObject = msg->GetGameObject();
	
	_arrIconDelegate.Emplace( msgObject.AddUObject(
			MessageGameObject::MINIMAP_SPAWN_ICON, this, &UGsMinimapIconHandler::OnSpawnIcon)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
			MessageGameObject::MINIMAP_DESPAWN_ICON, this, &UGsMinimapIconHandler::OnDespawnIcon)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
			MessageGameObject::MINIMAP_DESPAWN_ALL_ICON, this, &UGsMinimapIconHandler::OnDespawnAllIcon)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
			MessageGameObject::MINIMAP_DESPAWN_OBJ_ALL_ICON, this, &UGsMinimapIconHandler::OnDespawnObjAllIcon)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
			MessageGameObject::MINIMAP_NORMAL_MOSNTER_ON_OFF, this, &UGsMinimapIconHandler::OnNormalMonsterOnOff)
	);

	_arrIconDelegate.Emplace(msgObject.AddUObject(
			MessageGameObject::CLIENT_SPAWN_LIST_LOAD_FINISH, this, &UGsMinimapIconHandler::OnClientSpawnListLoadFinish)
	);

	_arrIconDelegate.Emplace(msgObject.AddUObject(
			MessageGameObject::QUEST_TARGET_NPC, this, &UGsMinimapIconHandler::OnQuestTargetNpc)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
			MessageGameObject::QUEST_TARGET_MONSTER, this, &UGsMinimapIconHandler::OnQuestTargetMonster)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
			MessageGameObject::QUEST_TARGET_POS, this, &UGsMinimapIconHandler::OnQuestTargetPos)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::QUEST_SUB_INTERATION_NPC, this, &UGsMinimapIconHandler::OnQuestSubInterationNpc)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::QUEST_SUB_INTERATION_NPC_LIST, this, &UGsMinimapIconHandler::OnQuestSubInterationNpcList)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_SPACE_CRACK_PORTAL, this, &UGsMinimapIconHandler::OnSpaceCrackPortal)
	);

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_PARTY_MEMBER, this, &UGsMinimapIconHandler::OnPartyMember)
	);

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_INVASION_NPC, this, &UGsMinimapIconHandler::OnInvasionNPC)
	);

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::ABNORMALITY_ALL_UPDATE, this, &UGsMinimapIconHandler::OnAbnormalityAllUpdate)
	);

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::MINIMAP_AUTO_MOVE_PATH_ICON_INFO, this, &UGsMinimapIconHandler::OnAutoMovePathIconInfo)
	);
	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::LOCAL_AUTOMOVE_STOP, this, &UGsMinimapIconHandler::OnLocalAutoMoveStop)
	);

	_delegateSpawnIconType =
		msg->GetUIMinimapSpawnType().AddUObject(
			MessageGameObject::MINIMAP_SPAWN_TYPE_ICON, this, &UGsMinimapIconHandler::OnSpawnIconType);

	_delegateIconTeamChange =
		msg->GetUIMinimapIconTeam().AddUObject(
			MessageContentMinimap::INVALIDATE_IFF, this, &UGsMinimapIconHandler::OnIconTeamChange);

	MUI& msgUI = msg->GetUI();

	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::INVADE_PORTAL_SPAWNSPOT_UPDATE,
		this, &UGsMinimapIconHandler::OnUpdateInvasionPortalSpawnInfo));

	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::INVADE_INVASIONTIME_UPDATE,
		this, &UGsMinimapIconHandler::OnUpdateInvasionPortalActive));
}
void UGsMinimapIconHandler::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();
	
	if (_arrIconDelegate.Num() != 0)
	{
		for (auto iter : _arrIconDelegate)
		{
			msg->GetGameObject().Remove(iter);
		}
		_arrIconDelegate.Empty();
	}

	msg->GetUIMinimapSpawnType().Remove(_delegateSpawnIconType);
	msg->GetUIMinimapIconTeam().Remove(_delegateIconTeamChange);

	if (_msgUIHandlerList.Num() != 0)
	{
		for (auto iter : _msgUIHandlerList)
		{
			msg->GetUI().Remove(iter);
		}
		_msgUIHandlerList.Empty();
	}
	
}

// 업데이트
void UGsMinimapIconHandler::Update(float In_delta, bool In_isLocalMoved)
{

	// 나, 이동형은 tick 호출
	for (TPair<int64, UGsMinimapIconBase*>& iter : _mapIcons)
	{
		if (iter.Value != nullptr)
		{
			// 모든 아이콘에게 전달
			if (In_isLocalMoved)
			{
				iter.Value->SetLocalPosDirty(true);
			}
			iter.Value->Tick(In_delta);
		}
	}

	// 파티 아이콘 갱신
	for (TPair<int64, UGsMinimapIconBase*>& iter : _mapPartyIcons)
	{
		if (iter.Value != nullptr)
		{
			// 모든 아이콘에게 전달
			if (In_isLocalMoved)
			{
				iter.Value->SetLocalPosDirty(true);
			}
			iter.Value->Tick(In_delta);
		}
	}

	// 2022/09/30 PKT - PK Detection Target Icon Update
	for (TPair<int64, UGsMinimapIconBase*>& iter : _mapPKDetectionTargetIcons)
	{
		if (iter.Value != nullptr)
		{
			// 모든 아이콘에게 전달
			if (In_isLocalMoved)
			{
				iter.Value->SetLocalPosDirty(true);
			}
			iter.Value->Tick(In_delta);
		}
	}

	for (TPair<EGsRegionInvadeNPCType, UGsMinimapIconBase*>& iter : _mapInvadeNPCIcons)
	{
		if (iter.Value != nullptr)
		{
			// 모든 아이콘에게 전달
			if (In_isLocalMoved)
			{
				iter.Value->SetLocalPosDirty(true);
			}
			iter.Value->Tick(In_delta);
		}
	}

	// 고정형은 실제로 이동시 dirty만 처리(내부 타이머에서 dirty 해제 처리)
	if (In_isLocalMoved)
	{
		for (TPair<int64, UGsMinimapIconBase*>& iter : _mapClientSpawnIcons)
		{
			if (iter.Value != nullptr)
			{
				iter.Value->SetLocalPosDirty(true);
			}
		}

		for (TPair<int, UGsMinimapIconBase*>& iterPos : _mapQuestPosIcons)
		{
			if (iterPos.Value != nullptr)
			{
				iterPos.Value->SetLocalPosDirty(true);
			}
		}

		for (TPair<uint64, UGsMinimapIconBase*>& iterPos : _mapBookmarkIcons)
		{
			if (iterPos.Value != nullptr)
			{
				iterPos.Value->SetLocalPosDirty(true);
			}
		}

		for (TPair<int64, UGsMinimapIconBase*>& iter : _mapSpaceCrackPortalIcons)
		{
			if (iter.Value != nullptr)
			{
				iter.Value->SetLocalPosDirty(true);
			}
		}

		for (TPair<int64, UGsMinimapIconBase*>& iter : _mapInvasionPortalIcons)
		{
			if (iter.Value != nullptr)
			{
				iter.Value->SetLocalPosDirty(true);
			}
		}

		for (TPair<int, UGsMinimapIconBase*>& iter: _mapAutoMovePathIcons)
		{
			if (iter.Value != nullptr)
			{
				iter.Value->SetLocalPosDirty(true);
			}
		}
		// 자동 이동 경로 삭제 처리(지나간 위치)
		InvalidateAutoMovePath();
	}
}
void UGsMinimapIconHandler::OnSpawnIconType(int32 In_type)
{
	UGsUIIconBase* iconWidget = CreateMapIcon(static_cast<EGsMapIconType>(In_type));

	// ui의 icon root에 붙이기
	GMessage()->GetUIMinimapIconWidget().SendMessage(MessageContentMinimap::ATTACH_WIDGET, iconWidget);
}

void UGsMinimapIconHandler::OnSpawnIcon(const IGsMessageParam* In_param)
{
	const FGsGameObjectMessageParamMapIcon* paramTarget =
		In_param->Cast<const FGsGameObjectMessageParamMapIcon>();

	if (nullptr == paramTarget ||
		paramTarget->_paramOwner == nullptr)
	{
		return;
	}

	// https://jira.com2us.com/jira/browse/CHR-18622
	// check abnormality hide
	
	// 0. not me
	// 1. is party
	// 2. is hide
	EGsGameObjectType targetObjType = paramTarget->_paramOwner->GetObjectType();
	if (targetObjType != EGsGameObjectType::LocalPlayer)
	{
		bool isMyParty = false;
		if (targetObjType == EGsGameObjectType::RemotePlayer)
		{
			isMyParty = paramTarget->_isPartyMember;
		}

		bool isHide = false;
		if (isMyParty == false &&
			paramTarget->_paramOwner != nullptr)
		{
			isHide = IsHideObj(paramTarget->_paramOwner);
		}

		// not party, hide: not make icon
		if (isHide == true)
		{
			return;
		}
	}
	UGsMinimapIconBase* icon = SpawnIcon(paramTarget->_paramOwner, paramTarget->_iconType);
	if (icon == nullptr)
	{
		return;
	}

	UGsMinimapIconOther* otherIcon = Cast<UGsMinimapIconOther>(icon);
	if (otherIcon == nullptr)
	{
		return;
	}
	otherIcon->SetIsPartyMember(paramTarget->_isPartyMember);

	// 파티원이면
	// 기존 생성되었던 파티 아이콘 삭제
	if (paramTarget->_isPartyMember == true)
	{		
		DelPartyMemberIcon(otherIcon->GetGameId());
	}
}
// 스폰 아이콘
UGsMinimapIconBase* UGsMinimapIconHandler::SpawnIcon(UGsGameObjectBase* In_target, EGsMapIconType In_iconType)
{
	if (nullptr == In_target)
	{
		return nullptr;
	}

	int64 gameId = In_target->GetGameId();
	UGsMinimapIconBase* findIcon = _mapIcons.FindRef(gameId);
	// 이미 있는거면
	if (findIcon != nullptr)
	{
		// 디스폰 처리
		DespawnIcon(gameId);
	}
	const FGsSchemaMinimapIconData* findTbl = FindMapIconData(In_iconType);
	if (nullptr == findTbl)
	{
		return nullptr;
	}

	float updateTime = findTbl->_updateTime;
	int depth = findTbl->_depth;

	UGsMinimapIconBase* makeIcon = nullptr;
	UGsUIIconBase* iconWidget = CreateMapIcon(In_iconType);

	UGsMinimapSceneImage* sceneImage = GSMinimap()->GetSceneImage();
	if (In_iconType == EGsMapIconType::LocalPlayer)
	{
		_iconLocal = NewObject<UGsMinimapIconLocal>();
		makeIcon = _iconLocal;
		
		if (sceneImage != nullptr)
		{
			// 지도 위치 갱신에 필요함
			sceneImage->SetIconLocal(_iconLocal);
		}
	}
	else
	{
		makeIcon = NewObject<UGsMinimapIconOther>();
	}

	UGsMinimapIconObject* objIcon = Cast<UGsMinimapIconObject>(makeIcon);
	objIcon->Initialize(In_target, iconWidget, In_iconType, updateTime, sceneImage, depth, gameId);

	// ui의 icon root에 붙이기
	GMessage()->GetUIMinimapIcon().SendMessage(MessageContentMinimap::ATTACH_ICON, makeIcon);

	_mapIcons.Add(gameId, makeIcon);
	_listSpawnIcons.Add(makeIcon);
#ifdef TEST_LOG
	GSLOG(Log, TEXT("add gameId: %d"), gameId);
#endif

	// 위치 갱신
	makeIcon->UpdatePos();
	if (In_iconType == EGsMapIconType::LocalPlayer &&
		_iconLocal != nullptr)
	{
		_iconLocal->UpdateRotation();
	}

	return makeIcon;
}
// 클라이언트 스폰 아이콘
void UGsMinimapIconHandler::ClientSpawnIcon(int64 In_id,  int In_tableId, EGsMapIconType In_iconType, FVector2D In_pos)
{
	UGsMinimapIconBase* findIcon = _mapClientSpawnIcons.FindRef(In_id);
	// 이미 있는거면
	if (findIcon != nullptr)
	{
		// 디스폰 처리
		ClientDespawnIcon(In_id);
	}

	UGsMinimapIconBase* makeIcon =
		MakeClientSpawnNpcIcon(In_id, In_iconType, In_pos, In_tableId);

	_mapClientSpawnIcons.Add(In_id, makeIcon);
	_listSpawnIcons.Add(makeIcon);
}
// 클라이언트 디스폰 아이콘
void UGsMinimapIconHandler::ClientDespawnIcon(int64 In_id)
{
	UGsMinimapIconBase* findIcon = _mapClientSpawnIcons.FindRef(In_id);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapClientSpawnIcons.Remove(In_id);
}

// 부모에게서 때기
void UGsMinimapIconHandler::ReleaseIcon(UGsMinimapIconBase* In_targetIcon)
{
	if (In_targetIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(In_targetIcon->GetIcon());
}

// game id로 디스폰
void UGsMinimapIconHandler::DespawnIcon(int64 In_gameID)
{
	UGsMinimapIconBase* findIcon = _mapIcons.FindRef(In_gameID);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	// 제거 성공했으면
	if (_mapIcons.Remove(In_gameID) > 0)
	{
#ifdef TEST_LOG
		GSLOG(Log, TEXT("remove gameId: %d"), gameId);
#endif
	}
}

// 나(local)는 안들어옴(remote, nonplayer만 호출)
void UGsMinimapIconHandler::OnDespawnIcon(const IGsMessageParam* In_param)
{

	const FGsGameObjectMessageParamMapIcon* paramTarget =
		In_param->Cast<const FGsGameObjectMessageParamMapIcon>();

	if (nullptr == paramTarget)
	{
		return;
	}

	int64 gameId = paramTarget->_gameId;
	DespawnIcon(gameId);


	// 해당 파티원 정보가 있으면 파티 아이콘 생성
	// 같은 맵일때만 처리
	if (FGsPartyMemberInfo* memberInfo = GSParty()->FindPartyMemberInfoByGameId(gameId))
	{			
		// 내가 다른맵갈때
		// 1. 모든 아이콘 지우고(참조 때문에 먼저)
		// 2. 모든 오브젝트 지울때 despawn이 들어올때 문제
		// SpawnMeComplete가 일어났는지 체크			
		if (GSGameObject()->GetIsSpawnMeComplete() == false)
		{
			return;
		}
			
		// 파티원이 다른맵 갈때
		// 디스폰 이유가 grid(시야밖)이 아니면 리턴
		if (paramTarget->_despawnReason != SPAWN_REASON::GRID)
		{
			return;
		}
										
		EGsMapIconType findIconType = EGsMapIconType::RemoteTeamRed;
		FGsMinimapFunc::GetPartyIconType(gameId, findIconType);
		MakePartyMemberIcon(memberInfo, findIconType);
	
	}
}
void UGsMinimapIconHandler::OnDespawnObjAllIcon(const IGsMessageParam*)
{
	ClearObjIcon();
	// 내가 다른채널 가면 타유저 정보 갱신(같은채널인지) 해야되므로 같이 날린다
	ClearPartyIcon();
	ClearSpaceCrackPortalIcon();
}
void UGsMinimapIconHandler::OnDespawnAllIcon(const IGsMessageParam*)
{
	ClearAllIcon();
}
// 모든 아이콘 삭제
void UGsMinimapIconHandler::ClearAllIcon()
{
	ClearObjIcon();
	ClearClientSpawnIcon();
	ClearQuestPosIcon();
	ClearBookmarkIcon();
	ClearPartyIcon();
	ClearSpaceCrackPortalIcon();
	ClearPKDetectionTargetIcon();
	ClearInvasionNPCIcon();
	ClearInvasionPortal();
	ClearAutoMovePathIcon();

	_listSpawnIcons.Empty();
}
// 오브젝트 아이콘 삭제
void UGsMinimapIconHandler::ClearObjIcon()
{
	int iconNum = _mapIcons.Num();
	if (iconNum > 0)
	{
		for (auto& iter : _mapIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapIcons.Empty();
	}
}

// 클라 스폰 아이콘 삭제
void UGsMinimapIconHandler::ClearClientSpawnIcon()
{
	int iconNum = _mapClientSpawnIcons.Num();
	if (iconNum > 0)
	{
		for (auto& iter : _mapClientSpawnIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapClientSpawnIcons.Empty();
	}
}
// 모든 퀘스트 위치 아이콘 삭제
void UGsMinimapIconHandler::ClearQuestPosIcon()
{
	int iconNum = _mapQuestPosIcons.Num();
	if (iconNum > 0)
	{
		for (auto& iter : _mapQuestPosIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapQuestPosIcons.Empty();
	}
}

// 모든 위치 즐겨찾기 아이콘 삭제
void UGsMinimapIconHandler::ClearBookmarkIcon()
{
	int iconNum = _mapBookmarkIcons.Num();
	if (iconNum > 0)
	{
		for (auto& iter : _mapBookmarkIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapBookmarkIcons.Empty();
	}
}
// 파티 아이콘 삭제
void UGsMinimapIconHandler::ClearPartyIcon()
{
	int iconNum = _mapPartyIcons.Num();
	if (iconNum > 0)
	{
		for (auto& iter : _mapPartyIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapPartyIcons.Empty();
	}
}
// 모든 시공의 틈새 아이콘 삭제
void UGsMinimapIconHandler::ClearSpaceCrackPortalIcon()
{
	if (_mapSpaceCrackPortalIcons.Num()>0)
	{
		for (auto iter: _mapSpaceCrackPortalIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapSpaceCrackPortalIcons.Empty();
	}
}
// 2022/09/29 PKT - PK 경계 대상 삭제
void UGsMinimapIconHandler::ClearPKDetectionTargetIcon()
{
	for (auto iter : _mapPKDetectionTargetIcons)
	{
		if (iter.Value != nullptr)
		{
			ReleaseIcon(iter.Value);
			iter.Value->Finalize();
			_listSpawnIcons.RemoveSwap(iter.Value);
			iter.Value = nullptr;
		}
	}

	_mapPKDetectionTargetIcons.Empty();
}
// 타입으로 아이콘 데이터 얻기
const FGsSchemaMinimapIconData* UGsMinimapIconHandler::FindMapIconData(EGsMapIconType In_type)
{
	const UGsTableMinimapIconData* tbl = Cast<UGsTableMinimapIconData>(FGsSchemaMinimapIconData::GetStaticTable());

	if (nullptr == tbl)
	{
		return nullptr;
	}

	const FGsSchemaMinimapIconData* findTbl = nullptr;
	tbl->FindRowById(In_type, findTbl);

	return findTbl;
}
void UGsMinimapIconHandler::OnIconTeamChange(const FGsMinimapIconTeamMessageParam* In_param)
{
	if (nullptr == In_param)
	{
		return;
	}

	// 나 팀 변경
	if (In_param->_isMe)
	{
		// 나머지 놈 다변경
		for (auto& iter : _mapIcons)
		{
			if (iter.Value != nullptr)
			{
				bool isLocalIcon = false;
				if (_iconLocal != nullptr)
				{
					// 게임아이디 같은거면 안함
					if (_iconLocal->GetGameId() == iter.Value->GetGameId())
					{
						isLocalIcon = true;
					}

				}
				// 로컬 아이콘 빼고 한다
				if (isLocalIcon == false)
				{
					ChangeRemoteTeam(iter.Value);
				}
			}
		}
	}
	else
	{
		UGsMinimapIconBase* findIcon = _mapIcons.FindRef(In_param->_gameId);
		if (findIcon == nullptr)
		{
			return;
		}
		UGsMinimapIconObject* objIcon = Cast<UGsMinimapIconObject>(findIcon);

		if (objIcon == nullptr)
		{
			return;
		}
		// 리모트 팀 변경
		ChangeRemoteTeam(findIcon);
	}
}
// 리모트 팀변경
void UGsMinimapIconHandler::ChangeRemoteTeam(UGsMinimapIconBase* In_icon)
{
	if (In_icon == nullptr)
	{
		return;
	}

	UGsMinimapIconOther* objIcon = Cast<UGsMinimapIconOther>(In_icon);

	if (objIcon == nullptr)
	{
		return;
	}
	// not change, if not red or blue or party
	if (objIcon->GetIconType() != EGsMapIconType::RemoteTeamRed &&
		objIcon->GetIconType() != EGsMapIconType::RemoteTeamBlue &&
		objIcon->GetIconType() != EGsMapIconType::Party1 &&
		objIcon->GetIconType() != EGsMapIconType::Party2 &&
		objIcon->GetIconType() != EGsMapIconType::Party3 &&
		objIcon->GetIconType() != EGsMapIconType::Party4&& 
		objIcon->GetIconType() != EGsMapIconType::Party1_OtherChannel &&
		objIcon->GetIconType() != EGsMapIconType::Party2_OtherChannel &&
		objIcon->GetIconType() != EGsMapIconType::Party3_OtherChannel &&
		objIcon->GetIconType() != EGsMapIconType::Party4_OtherChannel)
	{
		return;
	}

	// 아군 적근으로 처리하면
	EGsMapIconType iconType = (objIcon->IsEnemy()) ?
		EGsMapIconType::RemoteTeamRed : EGsMapIconType::RemoteTeamBlue;

	// 같은 타입이면 할필요 없음
	if (iconType == objIcon->GetIconType() ||
		objIcon->GetIsPartyMember() == true)
	{
		return;
	}
	objIcon->SetIconType(iconType);

	// 아이콘 해제
	ReleaseIcon(objIcon);
	UGsUIIconBase* iconWidget = CreateMapIcon(iconType);
	objIcon->SetIcon(iconWidget);

	// ui의 icon root에 붙이기
	GMessage()->GetUIMinimapIcon().SendMessage(MessageContentMinimap::ATTACH_ICON, objIcon);
}

// 일반 몬스터 on, off
void UGsMinimapIconHandler::OnNormalMonsterOnOff(const IGsMessageParam* In_param)
{
	const FGsPrimitiveInt32* param = In_param->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);
	_isNormalMonsterOn = isActive;


	// 끈다면
	if (_isNormalMonsterOn == false)
	{
		TArray<int64> arrDelIconId;
		// 기존 일반몬스터 아이콘 삭제
		for (auto& iter : _mapIcons)
		{
			if (iter.Value != nullptr)
			{
				if (iter.Value->GetIconType() == EGsMapIconType::Monster)
				{
					arrDelIconId.Add(iter.Value->GetGameId());
				}
			}
		}

		for (int64 delId : arrDelIconId)
		{
			DespawnIcon(delId);
		}
	}
	// 킨다면
	else
	{
		// 스폰 데이터에서 일반 몬스터로 아이콘 만든다
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			TArray<UGsGameObjectBase*> monsterList = mgr->FindObjectArray(EGsGameObjectType::NonPlayer);
			UGsGameObjectNonPlayer* nonPlayer = nullptr;
			const FGsSchemaNpcData* npcData = nullptr;
			const FGsSchemaNpcBalance* balanceTbl = nullptr;
			int tableId = 0;
			EGsMapIconType monsterType;
			for (UGsGameObjectBase* iter : monsterList)
			{
				nonPlayer = iter->CastGameObject<UGsGameObjectNonPlayer>();
				if (nullptr == nonPlayer)
				{
					continue;
				}

				balanceTbl = nonPlayer->GetNpcBalance();
				npcData = nonPlayer->GetNpcData();
				if (balanceTbl && npcData)
				{					
					if (class FGsGameObjectData* data = nonPlayer->GetData())
					{
						tableId = data->GetTableId();
					}

					monsterType = FGsMinimapFunc::GetMapIconTypeByCreatureRankType(npcData->grade, tableId);
					// 퀘스트나 보스는 이미 스폰 되어있을거므로 스폰할 필요 없다
					if (EGsMapIconType::Monster == monsterType)
					{
						SpawnIcon(nonPlayer, monsterType);
					}
				}
			}
		}
	}
}

// 만들수 있는 몬스터 아이콘인가
bool UGsMinimapIconHandler::CanMakeMonsterIcon(EGsMapIconType In_type)
{
	// 없는거면 실패
	if (In_type == EGsMapIconType::None)
	{
		return false;
	}

	// 일반 몬스터이면
	if (In_type == EGsMapIconType::Monster)
	{
		// 액티브 값으로 처리
		return _isNormalMonsterOn;
	}
	// 나머진 ok(보스, 퀘스트)
	return true;
}

// 클라 스폰 리스트 로드 끝
void UGsMinimapIconHandler::OnClientSpawnListLoadFinish(const IGsMessageParam* In_param)
{
	const FGsClientSpawnListLoadFinishMessageParam* param = 
		In_param->Cast<const FGsClientSpawnListLoadFinishMessageParam>();

	if (param == nullptr)
	{
		return;
	}

	// 기존 클라 스폰 아이콘 삭제
	ClearClientSpawnIcon();

	// 클라이언트 스폰 정보 가져오기(npc)
	TArray<TSharedPtr<FGsSpawnParam>> spawnList =
		param->_arrayNonPlayer;

	for (TSharedPtr<FGsSpawnParam> iter : spawnList)
	{
		const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(iter->_tId);
		if (nullptr == npcData)
		{
			continue;
		}

		if (npcData->npcFunctionType == NpcFunctionType::NONE)
		{
			continue;
		}


		// 서브 퀘스트면 서브 퀘스트 타입으로 처리		
		if (FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement())
		{
			if (questTargetManagement->IsQuestSubNpc(iter->_tId))
			{
				ClientSpawnIcon(iter->_gameId, iter->_tId, EGsMapIconType::SubQuest, FVector2D(iter->_pos));
				continue;
			}
		}


		const FGsSchemaNpcFunctionData* tableData =
			FGsMinimapFunc::GetNpcFunctionData(npcData->npcFunctionType);

		if (tableData == nullptr)
		{
			continue;
		}
		// 미니맵 출력이 아니면 표시하지 않는다
		if (tableData->_isRepresentIcon == false)
		{
			continue;
		}		

		// in invade map not show icon
		if (GGameData()->IsInvadeWorld() == true)
		{
			continue;
		}

		EGsMapIconType iconType =
			FGsMinimapFunc::GetMapIconTypeByNpcFunctionType(npcData->npcFunctionType);
		ClientSpawnIcon(iter->_gameId, iter->_tId,iconType, FVector2D(iter->_pos));
	}

	// 클라이언트 스폰 정보 가져오기(워프)
	TArray<TSharedPtr<FGsSpawnParam>> warpList = GSClientSpawn()->GetAllClientSpawnParamList(EGsGameObjectType::Warp);
	for (TSharedPtr<FGsSpawnParam> iter : warpList)
	{
		// 타입이 워프면 바로 만들기
		if (iter->_type == EGsGameObjectType::Warp)
		{
			ClientSpawnIcon(iter->_gameId, iter->_tId,EGsMapIconType::Portal,
				FVector2D(iter->_pos));
		}
	}

	// 북마크 아이콘 생성
	// ui construct 에서 하면 2번연속들어와서 
	// 여기서 처리
	LoadBookmarkData();

	// 현재맵, 현재채널의 시공의 틈새 포탈 아이콘 만든다
	MakeAllSpaceCrackPortalIcons();


	int currentLevelId = GLevel()->GetCurrentLevelId();
	TWeakPtr<FGsInvadeMap> invadeMap = GSInterServer()->GetInvadeMapData(currentLevelId);
	if (true == invadeMap.IsValid())
	{
		// make space crack icon by info
		MakeAllInvasionNPCIcons();
	}

	MakeIconInvasionPortal();
}


const FGsSchemaMinimapIconData* UGsMinimapIconHandler::GetTableData(EGsMapIconType InType) const
{
	const UGsTableMinimapIconData* table = Cast<UGsTableMinimapIconData>(FGsSchemaMinimapIconData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaMinimapIconData* row = nullptr;
	if (false == table->FindRowById(InType, row))
	{
		return nullptr;
	}

	return row;
}

UGsUIIconBase* UGsMinimapIconHandler::CreateMapIcon(EGsMapIconType InType)
{
	if (const FGsSchemaMinimapIconData* tableData = GetTableData(InType))
	{
		UGsUIIconBase* iconWidget = FGsIconFactory::CreateIcon(tableData->_iconShapeType);
		if (iconWidget)
		{
			if (tableData->_iconImagePath.IsValid())
			{
				iconWidget->SetIconImage(tableData->_iconImagePath);
			}

			iconWidget->SetIconImageColor(tableData->_iconColor);
		}

		return iconWidget;
	}

	return nullptr;
}

// 퀘스트 타겟(npc)
void UGsMinimapIconHandler::OnQuestTargetNpc(const IGsMessageParam* In_param)
{
	const FGsMinimapQuestTargetNpcParam* paramTarget =
		In_param->Cast<const FGsMinimapQuestTargetNpcParam>();

	if (paramTarget->_isAdd)
	{
		AddQuestNpc(paramTarget->_tableId, EGsMapIconType::MainQuest);
	}
	else
	{
		DelQuestNpc(paramTarget->_tableId, EGsMapIconType::MainQuest);
	}

}

// 퀘스트 타겟(몬스터)
void UGsMinimapIconHandler::OnQuestTargetMonster(const IGsMessageParam* In_param)
{
	const FGsMinimapQuestTargetMonsterParam* paramTarget =
		In_param->Cast<const FGsMinimapQuestTargetMonsterParam>();

	if (paramTarget->_isAdd)
	{
		AddQuestMonster(paramTarget->_tableId);
	}
	else
	{
		DelQuestMonster(paramTarget->_tableId);
	}
}

// 퀘스트 몬스터 추가
void UGsMinimapIconHandler::AddQuestMonster(TArray<int> In_arrId)
{
	FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
	if (questHandler == nullptr)
	{
		return;
	}

	FGsQuestTargetManagement* questTargetManagement = questHandler->GetQuestTargetManagement();
	if (questTargetManagement == nullptr)
	{
		return;
	}


	// 이미 존재하는 아이콘의 obj
	// 나중에 퀘스트 아이콘으로 바꾼다
	TArray<UGsGameObjectBase*> existIconObj;

	// 없는 몬스터들
	// 다 찾아서 만들자아
	TArray<int> notExistId;

	// 이렇게 기존 map icon 뒤지는 이유는
	// 이미 퀘스트 아이콘으로 만들어 져있는 놈일수도 있기 때문이다
	for (int iter : In_arrId)
	{
		bool isExist = false;
		for (auto& icon : _mapIcons)
		{
			if (icon.Value == nullptr)
			{
				continue;
			}

			// 추가인데 대상 tbl id가 이미 있으면
			if (icon.Value->GetTableId() == iter)
			{
				// 기존에 있던거다
				isExist = true;

				// 이미 퀘스트 아이콘이면 안해도 됨
				// 퀘스트 아이콘이 일반 몬스터면 다돌고 추가하자
				// map 내부 순회중이니까(추가하면 map이 증가되므로 error)
				if (icon.Value->GetIconType() == EGsMapIconType::Monster)
				{
					UGsMinimapIconObject* objIcon =
						Cast<UGsMinimapIconObject>(icon.Value);

					if (objIcon != nullptr &&
						existIconObj.Contains(objIcon->GetTargetObject()) == false)
					{
						existIconObj.Add(objIcon->GetTargetObject());
					}
				}
			}

		}
		// 다돌아 봤는데 없던데
		if (isExist == false)
		{
			if (notExistId.Contains(iter) == false)
			{
				// 추가
				notExistId.Add(iter);
			}
		}
	}

	// 몬스터 아이콘을 퀘스트 아이콘으로 만들기
	for (UGsGameObjectBase* iter : existIconObj)
	{
		UGsGameObjectNonPlayerBase* nonPlayer = iter->CastGameObject<UGsGameObjectNonPlayerBase>();
		if (nonPlayer != nullptr)
		{
			int npcTblId = nonPlayer->GetNpcTblId();

			QuestType outQuestType;
			if (true == questTargetManagement->IsQuestMonster(npcTblId, outQuestType))
			{
				EGsMapIconType iconType = EGsMapIconType::MainQuest;
				if (QuestType::REPEAT == outQuestType)
				{
					iconType = EGsMapIconType::RepeatQuest;
				}
			
				SpawnIcon(iter, iconType);
			}
		}
	}

	// 스포너에서 기존 스폰된 것들중에서 처리
	const FGsSchemaNpcBalance* balanceTbl = nullptr;
	const FGsSchemaNpcData* npcData = nullptr;
	UGsGameObjectNonPlayerBase* nonPlayer = nullptr;
	EGsMapIconType monsterType;
	for (int iter : notExistId)
	{
		TArray<UGsGameObjectBase*> arrTarget = GSGameObject()->FindObjectNonPlayerByNPCId(iter);

		for (UGsGameObjectBase* obj : arrTarget)
		{
			nonPlayer = obj->CastGameObject<UGsGameObjectNonPlayerBase>();
			if (nullptr == nonPlayer)
			{
				continue;
			}
			
			balanceTbl = nonPlayer->GetNpcBalance();			
			npcData = nonPlayer->GetNpcData();
			if (balanceTbl && npcData)
			{
				monsterType = FGsMinimapFunc::GetMapIconTypeByCreatureRankType(npcData->grade, iter);
				// minimap quest handler 에 이미 들어있을수도 있으므로
				// 퀘스트아니면 몬스터이면 
				if (EGsMapIconType::MainQuest == monsterType ||
					EGsMapIconType::Monster == monsterType)
				{
					SpawnIcon(obj, EGsMapIconType::MainQuest);
				}
				// 반복퀘스트 몬스터면 반복퀘 표시
				else if (EGsMapIconType::RepeatQuest == monsterType)
				{
					SpawnIcon(obj, EGsMapIconType::RepeatQuest);
				}
			}
		}
	}
}

// 퀘스트 몬스터 삭제
void UGsMinimapIconHandler::DelQuestMonster(TArray<int> In_arrId)
{
	TArray<UGsGameObjectBase*> delObjs;
	for (int iter : In_arrId)
	{
		for (auto& icon : _mapIcons)
		{
			if (icon.Value == nullptr)
			{
				continue;
			}

			// tbl id가 같고
			// 아이콘 타입이 메인 퀘스트이거나 반복 퀘스트이면
			if (icon.Value->GetTableId() == iter &&
				(icon.Value->GetIconType() == EGsMapIconType::MainQuest ||
				icon.Value->GetIconType() == EGsMapIconType::RepeatQuest))
			{
				UGsMinimapIconObject* objIcon =
					Cast<UGsMinimapIconObject>(icon.Value);
				UGsGameObjectBase* targetObj = objIcon->GetTargetObject();
				if (objIcon != nullptr &&
					targetObj != nullptr)
				{
					if (delObjs.Contains(targetObj) == false)
					{

						delObjs.Add(targetObj);
					}
				}
			}
		}
	}

	for (UGsGameObjectBase* iter : delObjs)
	{
		// 일반 몬스터 켜져있으면 일반몬스터 아이콘으로 변경
		if (_isNormalMonsterOn == true)
		{
			SpawnIcon(iter, EGsMapIconType::Monster);
		}
		// 아니면 삭제
		else
		{
			DespawnIcon(iter->GetData()->GetGameId());
		}
	}
}

// 퀘스트 npc 추가
void UGsMinimapIconHandler::AddQuestNpc(TArray<int> In_arrId, EGsMapIconType In_iconType)
{
	// 2020-07-07(화)
	// npc도 테이블 id로 함

	// npc는 미니맵에 대표 아이콘 밖에 없는데
	// 대표아이콘은 quest로 사용하지 않는다고 해서
	// 이미 있는건 무조건 퀘스트 아이콘이다
	// 그래서 있는건 무시

	// 이렇게 기존 map icon 뒤지는 이유는
	// 이미 퀘스트 아이콘으로 만들어 져있는 놈일수도 있기 때문이다

	// 없는 npc들
	// 다 찾아서 만들자아
	TArray<int> notExistId;
	for (int iter : In_arrId)
	{
		bool isExist = false;
		for (auto& icon : _mapIcons)
		{
			if (icon.Value == nullptr)
			{
				continue;
			}
			// 추가인데 대상 tbl id가 이미 있으면
			// 그리고 추가된 아이콘 타입과 같으면
			if (icon.Value->GetTableId() == iter &&
				icon.Value->GetIconType() == In_iconType)
			{
				// 기존에 있던거다
				isExist = true;
			}
		}

		if (isExist == false)
		{
			if (notExistId.Contains(iter) == false)
			{
				// 추가
				notExistId.Add(iter);
			}
		}
	}

	for (int iter : notExistId)
	{		
		TArray<UGsGameObjectBase*> arrTarget = GSGameObject()->FindObjectNonPlayerByNPCId(iter);

		for (UGsGameObjectBase* obj: arrTarget)
		{
			SpawnIcon(obj, In_iconType);
		}
	}
}
// 퀘스트 npc 삭제
void UGsMinimapIconHandler::DelQuestNpc(TArray<int> In_arrId, EGsMapIconType In_iconType)
{
	TArray<UGsGameObjectBase*> delObjs;
	for (int iter : In_arrId)
	{
		for (auto& icon : _mapIcons)
		{
			if (icon.Value == nullptr)
			{
				continue;
			}

			// tbl id가 같고
			// 아이콘 타입이 들어온 타입과 같으면
			if (icon.Value->GetTableId() == iter &&
				icon.Value->GetIconType() == In_iconType)
			{
				UGsMinimapIconObject* objIcon =
					Cast<UGsMinimapIconObject>(icon.Value);
				UGsGameObjectBase* targetObj = objIcon->GetTargetObject();
				if (objIcon != nullptr &&
					targetObj != nullptr)
				{
					if (delObjs.Contains(targetObj) == false)
					{

						delObjs.Add(targetObj);
					}
				}
			}
		}
	}

	for (UGsGameObjectBase* iter : delObjs)
	{	
		DespawnIcon(iter->GetData()->GetGameId());
	}
}
// 서브 퀘스트 npc 추가
// 기존엔 AddQuestNpc로 같이 썼는데
// (game object 스폰, 디스폰시 아이콘 생성, 삭제)
// 서브 퀘스트 아이콘이 멀리서도 보여야 한다고 해서
// client spawn 처럼 맵 진입시 스폰, 맵이동시 디스폰, 퀘스트 갱신시 스폰, 디스폰
// 처리로 바꿈
void UGsMinimapIconHandler::AddSubQuestNpc(TArray<int> In_arrId, EGsMapIconType In_iconType)
{
	// 2020-07-07(화)
	// npc도 테이블 id로 함

	// npc는 미니맵에 대표 아이콘 밖에 없는데
	// 대표아이콘은 quest로 사용하지 않는다고 해서
	// 이미 있는건 무조건 퀘스트 아이콘이다
	// 그래서 있는건 무시

	// 이렇게 기존 map icon 뒤지는 이유는
	// 이미 퀘스트 아이콘으로 만들어 져있는 놈일수도 있기 때문이다

	// 없는 npc들
	// 다 찾아서 만들자아
	TArray<int> notExistId;
	for (int iter : In_arrId)
	{
		bool isExist = false;
		for (auto& icon : _mapClientSpawnIcons)
		{
			if (icon.Value == nullptr)
			{
				continue;
			}
			// 추가인데 대상 tbl id가 이미 있으면
			// 그리고 추가된 아이콘 타입과 같으면
			if (icon.Value->GetTableId() == iter &&
				icon.Value->GetIconType() == In_iconType)
			{
				// 기존에 있던거다
				isExist = true;
			}
		}

		if (isExist == false)
		{
			if (notExistId.Contains(iter) == false)
			{
				// 추가
				notExistId.Add(iter);
			}
		}
	}

	// 모든 client spawn npc 가져옴
	TArray<TSharedPtr<FGsSpawnParam>> npcList = GSClientSpawn()->GetAllClientSpawnParamList(EGsGameObjectType::NonPlayer);
	for (auto iter: npcList)
	{
		// 만들어야 하는 id면 생성
		if (notExistId.Contains(iter->_tId) == true)
		{
			ClientSpawnIcon(iter->_gameId, iter->_tId,EGsMapIconType::SubQuest,
				FVector2D(iter->_pos));
		}
	}

}
// 서브 퀘스트 npc 삭제
void UGsMinimapIconHandler::DelSubQuestNpc(TArray<int> In_arrId, EGsMapIconType In_iconType)
{
	TArray<int64> delGameIds;
	for (int iter : In_arrId)
	{
		for (auto& icon : _mapClientSpawnIcons)
		{
			if (icon.Value == nullptr)
			{
				continue;
			}

			// tbl id가 같고
			// 아이콘 타입이 들어온 타입과 같으면
			if (icon.Value->GetTableId() == iter &&
				icon.Value->GetIconType() == In_iconType)
			{
				if (delGameIds.Contains(icon.Key) == false)
				{
					delGameIds.Add(icon.Key);
				}			
			}
		}
	}

	for (int64 iterId : delGameIds)
	{
		ClientDespawnIcon(iterId);
	}

}
// 퀘스트 타겟(위치)
void UGsMinimapIconHandler::OnQuestTargetPos(const IGsMessageParam* In_param)
{
	const FGsMinimapQuestTargetPosParam* paramTarget =
		In_param->Cast<const FGsMinimapQuestTargetPosParam>();


	if (paramTarget->_isAdd)
	{
		AddQuestPos(paramTarget->_posData);
	}
	else
	{
		DelQuestPos(paramTarget->_posData);
	}
}

void UGsMinimapIconHandler::OnQuestSubInterationNpc(const IGsMessageParam* In_param)
{
	const FGsMinimapQuesSubInterationNpcParam* paramTarget =
		In_param->Cast<const FGsMinimapQuesSubInterationNpcParam>();

	TArray<int32> arrId;
	arrId.Add(paramTarget->_tableId);

	if (paramTarget->_isMinimapAdd)
	{		
		AddSubQuestNpc(arrId, EGsMapIconType::SubQuest);
	}
	else
	{
		DelSubQuestNpc(arrId, EGsMapIconType::SubQuest);
	}
}

void UGsMinimapIconHandler::OnQuestSubInterationNpcList(const IGsMessageParam* In_param)
{
	const FGsQuesSubInterationNpcMarkParam* paramTarget =
		In_param->Cast<const FGsQuesSubInterationNpcMarkParam>();

	TArray<FGsSubQuestNpcMarkData> MarkDataList = paramTarget->_npcMarkDataList;
	for (FGsSubQuestNpcMarkData& data : MarkDataList)
	{
		TArray<int32> arrId;
		arrId.Add(data.GetNpcId());

		if (data.GetIsMiniMapOn())
		{
			AddSubQuestNpc(arrId, EGsMapIconType::SubQuest);
		}
		else
		{
			DelSubQuestNpc(arrId, EGsMapIconType::SubQuest);
		}
	}
}

// 퀘스트 위치 추가
void UGsMinimapIconHandler::AddQuestPos(const TArray<int>& In_arrData)
{
	int32 currentLevelId = 0;

	if (UGsLevelManager* level = GLevel())
	{
		currentLevelId = level->GetCurrentLevelId();
	}

	for (int iter : In_arrData)
	{
		// 스폰 하지 않았으면
		if (_mapQuestPosIcons.Contains(iter) == false)
		{
			FVector pos = FVector::ZeroVector;
			FRotator rot = FRotator::ZeroRotator;

			if (true == UGsLevelUtil::TryGetSpot(currentLevelId, iter, pos, rot))
			{
				MakeQuestPosIcon(iter, FVector2D(pos));
			}
		}
	}
}

// 퀘스트 위치 삭제
void UGsMinimapIconHandler::DelQuestPos(const TArray<int>& In_arrData)
{
	for (int iter : In_arrData)
	{
		DelQuestPosIcon(iter);
	}
}
// 퀘스트 위치 아이콘 생성
void UGsMinimapIconHandler::MakeQuestPosIcon(int In_spotId, FVector2D In_pos)
{
	EGsMapIconType questPosType = EGsMapIconType::MainQuest;

	UGsMinimapIconBase* findIcon = _mapQuestPosIcons.FindRef(In_spotId);
	// 이미 있는거면
	if (findIcon != nullptr)
	{
		// 디스폰 처리
		DelQuestPosIcon(In_spotId);
	}

	UGsMinimapIconBase* makeIcon =
		MakeClientSpawnNpcIcon(In_spotId, questPosType, In_pos);

	if (makeIcon == nullptr)
	{
		return;
	}

	_mapQuestPosIcons.Add(In_spotId, makeIcon);
	_listSpawnIcons.Add(makeIcon);
}
// 퀘스트 위치 아이콘 삭제
void UGsMinimapIconHandler::DelQuestPosIcon(int In_spotId)
{
	UGsMinimapIconBase* findIcon = _mapQuestPosIcons.FindRef(In_spotId);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapQuestPosIcons.Remove(In_spotId);
}


// 북마크 아이콘 생성
void UGsMinimapIconHandler::MakeBookmarkIcon(uint64 In_bookmarkId, FVector2D In_pos, int In_mapId)
{
	int32 currentLevelId = 0;
	if (UGsLevelManager* level = GLevel())
	{
		currentLevelId = level->GetCurrentLevelId();
	}

	if (currentLevelId != In_mapId)
	{
		return;
	}


	EGsMapIconType iconType = EGsMapIconType::Bookmark;

	UGsMinimapIconBase* findIcon = _mapBookmarkIcons.FindRef(In_bookmarkId);
	// 이미 있는거면
	if (findIcon != nullptr)
	{
		// 디스폰 처리
		DelBookmarkIcon(In_bookmarkId);
	}

	UGsMinimapIconBase* makeIcon =
		MakeClientSpawnNpcIcon(In_bookmarkId, iconType, In_pos);

	if (makeIcon == nullptr)
	{
		return;
	}

	_mapBookmarkIcons.Add(In_bookmarkId, makeIcon);
	_listSpawnIcons.Add(makeIcon);
}
// 북마크 아이콘 삭제
void UGsMinimapIconHandler::DelBookmarkIcon(uint64 In_bookmarkId)
{
	UGsMinimapIconBase* findIcon = _mapBookmarkIcons.FindRef(In_bookmarkId);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapBookmarkIcons.Remove(In_bookmarkId);
}


// 클라이언트 스폰 npc, 포털, book mark 생성용
UGsMinimapIconBase* UGsMinimapIconHandler::MakeClientSpawnNpcIcon(
	int64 In_id, EGsMapIconType In_iconType, FVector2D In_pos,
	int In_tableId)
{
	const FGsSchemaMinimapIconData* findTbl = FindMapIconData(In_iconType);
	if (nullptr == findTbl)
	{
		return nullptr;
	}

	int depth = findTbl->_depth;
	float updateTime = findTbl->_updateTime;

	UGsUIIconBase* iconWidget = CreateMapIcon(In_iconType);

	UGsMinimapIconNpc* makeIcon = NewObject<UGsMinimapIconNpc>();

	UGsMinimapSceneImage* sceneImage = GSMinimap()->GetSceneImage();
	makeIcon->Initialize(iconWidget, In_iconType,
		updateTime,
		sceneImage, depth, In_id, In_pos,
		In_tableId);

	// ui의 icon root에 붙이기
	GMessage()->GetUIMinimapIcon().SendMessage(MessageContentMinimap::ATTACH_ICON, makeIcon);

	// 위치 갱신
	makeIcon->UpdatePos();

	return makeIcon;
}

// 북마크 데이터 다받음
void UGsMinimapIconHandler::LoadBookmarkData()
{
	TArray<FGsBookmarkData*> arrData = GSBookmark()->GetBookmarkData();
	if (arrData.Num() == 0)
	{
		return;
	}
	for (FGsBookmarkData* iter: arrData)
	{
		if (nullptr == iter)
		{
			continue;
		}

		// 만듬
		MakeBookmarkIcon(iter->GetId(), FVector2D(iter->GetPos()), iter->GetMapId());
	}
}

// 파티 멤버 (추가, 삭제)
void UGsMinimapIconHandler::OnPartyMember(const IGsMessageParam* In_param)
{
	const FGsUIMapPartyMemberParam* paramTarget =
		In_param->Cast<const FGsUIMapPartyMemberParam>();

	if (paramTarget->_isAdd == true) 
	{
		AddPartyMember(paramTarget->_gameId);
	}
	else
	{
		DelPartyMember(paramTarget->_gameId);
	}
}


// 파티 멤버 추가
void UGsMinimapIconHandler::AddPartyMember(const TArray<int64>& In_arrId)
{
	int32 currentLevelId = 0;
	if (UGsLevelManager* level = GLevel())
	{
		currentLevelId = level->GetCurrentLevelId();
	}

	for (int64 iter: In_arrId)
	{

		EGsMapIconType iconType = EGsMapIconType::RemoteTeamRed;
		FGsMinimapFunc::GetPartyIconType(iter, iconType);

		// gameobject에 있으면 object 타입 아이콘으로 만든다
		UGsGameObjectBase* findObj =
			GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, iter);

		if (findObj != nullptr)
		{
			UGsMinimapIconBase* icon = SpawnIcon(findObj, iconType);
			if (icon == nullptr)
			{
				return;
			}

			UGsMinimapIconOther* otherIcon = Cast<UGsMinimapIconOther>(icon);
			if (otherIcon == nullptr)
			{
				return;
			}
			otherIcon->SetIsPartyMember(true);
		}
		else
		{
			// game object에 없으면 info 기준으로 만든다
			// 현재 맵이랑 다르면 처리 안함
			// 개인 이벤트 채널이면 생성 안함
			FGsPartyMemberInfo* info = GSParty()->FindPartyMemberInfoByGameId(iter);
			if (nullptr == info ||
				info->GetMapId() != currentLevelId||
				GLevel()->IsInstanceSingleMap() == true)
			{
				continue;
			}		
			
			MakePartyMemberIcon(info, iconType);
		}
	}
}
// 파티 멤버 삭제
void UGsMinimapIconHandler::DelPartyMember(const TArray<int64>& In_arrId)
{
	for (int64 iter : In_arrId)
	{
		// gameobject에 있으면 object 타입 아이콘으로 삭제한다
		UGsGameObjectBase* findObj =
			GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, iter);

		if (findObj != nullptr)
		{			
			DespawnIcon(iter);

			// check hide
			bool isHide = IsHideObj(findObj);
			if (isHide == false)
			{
				EGsMapIconType iconType = EGsMapIconType::RemoteTeamRed;
				FGsMinimapFunc::GetRemoteIffIconType(iter, iconType);
				// make normal icon
				SpawnIcon(findObj, iconType);
			}
		}
		else
		{
			// 아니면 info 기준 삭제
			DelPartyMemberIcon(iter);
		}
	}
}

void UGsMinimapIconHandler::AddPKDetectionTargetIcon(UserDBId InId, int64 InGameId, const TArray<FGsPKDetectionEffectInfo>& InEffectInfoSet)
{
	// 2022/09/29 PKT - Find Remote Player
	UGsGameObjectBase* findRemotePlayer = GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, InGameId);
	if (nullptr == findRemotePlayer)
	{
		GSLOG(Error, TEXT("findRemotePlayer"));
		return;
	}

	if (true == _mapPKDetectionTargetIcons.Contains(InId))
	{
		GSLOG(Error, TEXT("already Detector Target : [%lu]"), InId);
		return;
	}

	const FGsSchemaMinimapIconData* schemaMinimapIconData = FindMapIconData(EGsMapIconType::DetectionTarget);
	if (nullptr == schemaMinimapIconData)
	{
		GSLOG(Error, TEXT("not Find Schema Data : EGsMapIconType::DetectionTarget"));
		return;
	}

	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
	{
		GSLOG(Error, TEXT("nullptr == serverOption"));
		return;
	}

	UGsMinimapIconDetectionTarget* makeIcon = NewObject<UGsMinimapIconDetectionTarget>();
	makeIcon->SetData(
		findRemotePlayer
		, CreateMapIcon(EGsMapIconType::DetectionTarget)
		, GSMinimap()->GetSceneImage()
		, EGsMapIconType::DetectionTarget
		, schemaMinimapIconData->_updateTime
		, InGameId
		, schemaMinimapIconData->_depth
		, InEffectInfoSet
	);
	
	// 2022/09/29 PKT - Attach
	GMessage()->GetUIMinimapIcon().SendMessage(MessageContentMinimap::ATTACH_ICON, makeIcon);

	_mapPKDetectionTargetIcons.Add(InGameId, makeIcon);
	_listSpawnIcons.Add(makeIcon);

	// 2022/09/30 PKT - 경계 대상이 추가 되면 기존 아이콘을 숨긴다.
	if (_mapIcons.Contains(InGameId))
	{
		_mapIcons[InGameId]->SetIconHidden(true);
	}
}

void UGsMinimapIconHandler::DelPKDetectionTargetIcon(int64 InGameId)
{
	UGsMinimapIconBase* findIcon = _mapPKDetectionTargetIcons.FindRef(InGameId);
	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapPKDetectionTargetIcons.Remove(InGameId);

	// 2022/09/30 PKT - 경계 대상이 해제 되면 기존 아이콘을 보여준다.
	if (_mapIcons.Contains(InGameId))
	{
		_mapIcons[InGameId]->SetIconHidden(false);
	}
}

// 파티 스폰 아이콘
void UGsMinimapIconHandler::MakePartyMemberIcon(FGsPartyMemberInfo* In_partyInfo, EGsMapIconType In_iconType)
{
	int64 gameId = In_partyInfo->GetGameId();

	UGsMinimapIconBase* findIcon = _mapPartyIcons.FindRef(gameId);
	if (findIcon != nullptr)
	{
		// 디스폰 처리
		DelPartyMemberIcon(gameId);
	}

	const FGsSchemaMinimapIconData* findTbl = FindMapIconData(In_iconType);
	if (nullptr == findTbl)
	{
		return;
	}

	int depth = findTbl->_depth;
	float updateTime = findTbl->_updateTime;

	UGsUIIconBase* iconWidget = CreateMapIcon(In_iconType);
	UGsMinimapIconParty* makeIcon = NewObject<UGsMinimapIconParty>();
	// 화살표 위젯
	UUserWidget* makeArrowWidget = FGsMinimapFunc::MakeArrowWidget();

	UGsUIIconMinimap* minimapIcon = Cast<UGsUIIconMinimap>(iconWidget);
	if (minimapIcon != nullptr &&
		makeArrowWidget != nullptr)
	{
		minimapIcon->AddCanvas(makeArrowWidget);
		makeArrowWidget->SetRenderTranslation(FVector2D::ZeroVector);
	}
	
	UGsMinimapSceneImage* sceneImage = GSMinimap()->GetSceneImage();
	makeIcon->Initialize(In_partyInfo
		,iconWidget, In_iconType,
		updateTime,
		sceneImage, depth, gameId, makeArrowWidget);

	// ui의 icon root에 붙이기
	GMessage()->GetUIMinimapIcon().SendMessage(MessageContentMinimap::ATTACH_ICON, makeIcon);

	// 위치 갱신
	makeIcon->UpdateObjectTransform();

	_listSpawnIcons.Add(makeIcon);
	_mapPartyIcons.Add(gameId, makeIcon);
}
// 파티 디스폰 아이콘
void UGsMinimapIconHandler::DelPartyMemberIcon(int64 In_id)
{
	UGsMinimapIconBase* findIcon = _mapPartyIcons.FindRef(In_id);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapPartyIcons.Remove(In_id);
}

void UGsMinimapIconHandler::OnSpaceCrackPortal(const IGsMessageParam* In_param)
{
	const FGsMapSpaceCrackAlarmParam* paramTarget =
		In_param->Cast<const FGsMapSpaceCrackAlarmParam>();


#ifdef LOG_PRINT_MINIMAP
	GSLOG(Log, TEXT("[UGsMinimapIconHandler] OnSpaceCrackPortal gameId: %llu, channelId: %d, levelId: %d, isAdd: %d"), 
		paramTarget->_gameId, paramTarget->_channelId,paramTarget->_regionMapId, paramTarget->_isAdd);
#endif

	// 현재 채널과 다르면 처리 안함
	int currentChannel = GGameData()->GetCurrentChannel();
#ifdef LOG_PRINT_MINIMAP
	GSLOG(Log, TEXT("[UGsMinimapIconHandler] OnSpaceCrackPortal currentChannel: %d"),
		currentChannel);
#endif

	int32 currentLevelId = 0;
	if (UGsLevelManager* level = GLevel())
	{
		currentLevelId = level->GetCurrentLevelId();
	}
#ifdef LOG_PRINT_MINIMAP
	GSLOG(Log, TEXT("[UGsMinimapIconHandler] OnSpaceCrackPortal currentLevelId: %d"),
		currentLevelId);
#endif
	if (currentLevelId != paramTarget->_regionMapId)
	{
		return;
	}

	if (paramTarget->_isAdd == true)
	{
		MakeSpaceCrackPortal(paramTarget->_regionMapId, FVector2D(paramTarget->_pos));
	}
	else
	{
		DelSpaceCrackPortal(paramTarget->_regionMapId);
	}
}

// 시공의 틈새 포털 아이콘 생성
void UGsMinimapIconHandler::MakeSpaceCrackPortal(int64 In_gameId, FVector2D In_pos)
{
	EGsMapIconType iconType = EGsMapIconType::SpaceCrackPortal;

	UGsMinimapIconBase* findIcon = _mapSpaceCrackPortalIcons.FindRef(In_gameId);
	// 이미 있는거면
	if (findIcon != nullptr)
	{
		// 디스폰 처리
		DelSpaceCrackPortal(In_gameId);
	}

	UGsMinimapIconBase* makeIcon =
		MakeClientSpawnNpcIcon(In_gameId, iconType, In_pos);

	if (makeIcon == nullptr)
	{
		return;
	}

	_mapSpaceCrackPortalIcons.Add(In_gameId, makeIcon);
	_listSpawnIcons.Add(makeIcon);
}
// 시공의 틈새 포털 아이콘 삭제
void UGsMinimapIconHandler::DelSpaceCrackPortal(int64 In_gameId)
{
	UGsMinimapIconBase* findIcon = _mapSpaceCrackPortalIcons.FindRef(In_gameId);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapSpaceCrackPortalIcons.Remove(In_gameId);
}

// 모든 시공의 틈 포탈 아이콘 만들기(현재맵)
void UGsMinimapIconHandler::MakeAllSpaceCrackPortalIcons()
{
	// 현재 채널과 다르면 처리 안함
	int currentChannel = GGameData()->GetCurrentChannel();
	int32 currentLevelId = 0;
	if (UGsLevelManager* level = GLevel())
	{
		currentLevelId = level->GetCurrentLevelId();
	}

	if (false == GGameData()->IsInvadeWorld())
	{
		const TMap<MapId, TSharedPtr<FGsSpaceCrackAlarmInfo>>& infos = GSSpaceCrack()->GetSpaceCrackAlarmInfoMap();
		for (auto& iter : infos)
		{
			if (false == iter.Value.IsValid() ||
				iter.Value->_regionMapId != currentLevelId)
			{
				continue;
			}

			MakeSpaceCrackPortal(iter.Value->_regionMapId, FVector2D(iter.Value->_pos));
		}
	}	
}

// maske invasion npc icon(guardian, monster, wagon)
void UGsMinimapIconHandler::MakeInvasionNPCIcon(FGsRegionInvadeIconInfo* In_iconInfo, EGsMapIconType In_iconType)
{
	if (In_iconInfo == nullptr)
	{
		return;
	}

	EGsRegionInvadeNPCType npcType = In_iconInfo->_NPCType;

	UGsMinimapIconBase* findIcon = _mapInvadeNPCIcons.FindRef(npcType);
	// already exist->del
	if (findIcon != nullptr)
	{
		DelInvasionNPCIcon(npcType);
	}
	const FGsSchemaMinimapIconData* findTbl = FindMapIconData(In_iconType);
	if (nullptr == findTbl)
	{
		return;
	}

	int depth = findTbl->_depth;
	float updateTime = findTbl->_updateTime;

	UGsUIIconBase* iconWidget = CreateMapIcon(In_iconType);

	UGsMinimapIconInvasionNPC* makeIcon = NewObject<UGsMinimapIconInvasionNPC>();

	UGsMinimapSceneImage* sceneImage = GSMinimap()->GetSceneImage();

	int64 gameId = In_iconInfo->_gameId;

	makeIcon->Initialize(In_iconInfo,
		iconWidget,
		In_iconType,
		updateTime,
		sceneImage,
		depth,
		gameId);

	// ui의 icon root에 붙이기
	GMessage()->GetUIMinimapIcon().SendMessage(MessageContentMinimap::ATTACH_ICON, makeIcon);

	makeIcon->UpdatePos();

	_listSpawnIcons.Add(makeIcon);

	_mapInvadeNPCIcons.Add(npcType, makeIcon);
}
// delete invasion npc icon (guardian, monster, wagon)
void UGsMinimapIconHandler::DelInvasionNPCIcon(EGsRegionInvadeNPCType In_npcType)
{
	UGsMinimapIconBase* findIcon = _mapInvadeNPCIcons.FindRef(In_npcType);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapInvadeNPCIcons.Remove(In_npcType);
}

// clear all invasion npc icon
void UGsMinimapIconHandler::ClearInvasionNPCIcon()
{
	int iconNum = _mapInvadeNPCIcons.Num();
	if (iconNum > 0)
	{
		for (auto& iter : _mapInvadeNPCIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapInvadeNPCIcons.Empty();
	}
}

void UGsMinimapIconHandler::AddInvasionNPC(const TArray<EGsRegionInvadeNPCType> In_arrayType)
{
	for (EGsRegionInvadeNPCType iterType : In_arrayType)
	{
		FGsRegionInvadeIconInfo* findInfo = GSWorldMap()->GetRegionInvadeInfoByNPCType(iterType);
		if (findInfo == nullptr)
		{
			continue;
		}

		EGsMapIconType iconType = FGsMapFunc::GetInvasionNPCMiniMapIconType(findInfo->_NPCType);

		if (iconType == EGsMapIconType::None)
		{
			continue;
		}

		MakeInvasionNPCIcon(findInfo, iconType);
	}
}
void UGsMinimapIconHandler::DelInvasionNPC(const TArray<EGsRegionInvadeNPCType> In_arrayType)
{
	for (EGsRegionInvadeNPCType iterType : In_arrayType)
	{
		DelInvasionNPCIcon(iterType);
	}
}

// invasion npc (add, del)
void UGsMinimapIconHandler::OnInvasionNPC(const IGsMessageParam* In_param)
{
	const FGsMapInvasionNPCParam* paramTarget = In_param->Cast<const FGsMapInvasionNPCParam>();

	if (paramTarget->_isAdd)
	{
		AddInvasionNPC(paramTarget->_arrayNPCType);
	}
	else
	{
		DelInvasionNPC(paramTarget->_arrayNPCType);
	}
}

// all invasion npc icons
void UGsMinimapIconHandler::MakeAllInvasionNPCIcons()
{
	TMap<EGsRegionInvadeNPCType, FGsRegionInvadeIconInfo*> invadeInfo =
		GSWorldMap()->GetRegionInvadeInfo();

	if (invadeInfo.Num() == 0)
	{
		return;
	}

	for (auto& iter : invadeInfo)
	{
		if (iter.Value == nullptr)
		{
			continue;
		}

		EGsMapIconType iconType = FGsMapFunc::GetInvasionNPCMiniMapIconType(iter.Value->_NPCType);

		if (iconType == EGsMapIconType::None)
		{
			continue;
		}


		MakeInvasionNPCIcon(iter.Value, iconType);
	}
}
// abnormality update
void UGsMinimapIconHandler::OnAbnormalityAllUpdate(const IGsMessageParam* In_param)
{
	const FGsGameObjectMessageParamAbnormality* param = In_param->Cast<const FGsGameObjectMessageParamAbnormality>();
	if (param == nullptr
		|| param->_paramOwner == nullptr)
	{
		return;
	}

	int64 paramTargetGameId = param->_paramOwner->GetGameId();
	// not setting: eror or skill preview
	if (paramTargetGameId == 0)
	{
		return;
	}
	// not check me
	if (param->_paramOwner->GetObjectType() == EGsGameObjectType::LocalPlayer)
	{
		return;
	}

	bool isParty = false;
	if (GSParty()->GetIsInParty() == true)
	{
		if (param->_paramOwner != nullptr &&
			param->_paramOwner->GetObjectType() == EGsGameObjectType::RemotePlayer)
		{
			isParty = GSParty()->IsInPartyGameId(param->_paramOwner->GetGameId());
		}
	}

	if (isParty == true)
	{
		return;
	}
	// not hide , not use
	if (param->_effectType != AbnormalityEffectType::UTIL_HIDING)
	{
		return;
	}
	// delete icon
	// remove false -> not remove -> make abnormality -> hide
	if (param->_bIsRemove == false)
	{
		DespawnIcon(paramTargetGameId);
	}
	// make icon
	else
	{
		EGsMapIconType iconType = EGsMapIconType::None;
		// remote
		if (param->_paramOwner->GetObjectType() == EGsGameObjectType::RemotePlayer)
		{
			iconType = UGsGameObjectUtil::IsEnemy(param->_paramOwner)? EGsMapIconType::RemoteTeamRed : EGsMapIconType::RemoteTeamBlue;
		}
		else if (param->_paramOwner->GetObjectType() == EGsGameObjectType::NonPlayerBase)
		{
			UGsGameObjectNonPlayerBase* npc = param->_paramOwner->CastGameObject<UGsGameObjectNonPlayerBase>();
			if (npc == nullptr)
			{
				return;
			}
			int tblId = npc->GetNpcTblId();

			const FGsSchemaNpcData* npcData = npc->GetNpcData();

			if (npcData == nullptr)
			{
				return;
			}

			iconType =
				FGsMinimapFunc::GetMapIconTypeByCreatureRankType(npcData->grade, tblId);
								
			bool isShowInMinimap = CanMakeMonsterIcon(iconType);
			if (isShowInMinimap == false)
			{
				return;
			}
			
		}
		if (iconType != EGsMapIconType::None)
		{
			SpawnIcon(param->_paramOwner, iconType);
		}
	}
}

// check abnormality hide obj
bool UGsMinimapIconHandler::IsHideObj(UGsGameObjectBase* In_obj)
{
	if (In_obj == nullptr)
	{
		return false;
	}
	bool isHide = false;
	if (FGsAbnormalityHandlerBase* abnormalityHandler = In_obj->GetAbnormalityHandler())
	{
		isHide = abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_HIDING);
	}
	return isHide;
}

// make all invasion portal
void UGsMinimapIconHandler::MakeIconInvasionPortal()
{
	if (true == GGameData()->IsInvadeWorld())
	{
		return;
	}

	int32 currentLevelId = 0;
	if (UGsLevelManager* level = GLevel())
	{
		currentLevelId = level->GetCurrentLevelId();
	}

	TSharedPtr<FGsInvadeMap> invadeMapdata = GSInterServer()->GetInvadeMapData(currentLevelId).Pin();
	if (false == invadeMapdata.IsValid())
	{
		return;
	}
	
	if (false == invadeMapdata->_activePortal)
	{
		return;
	}

	for (FGsSchemaSpotInfo& spot : invadeMapdata->_spawnPortals)
	{
		MakeInvasionPortal(spot.id, FVector2D(spot.pos));
	}		
}


void UGsMinimapIconHandler::MakeInvasionPortal(int64 In_spotId, FVector2D In_pos)
{
	
	UGsMinimapIconBase* findIcon = _mapInvasionPortalIcons.FindRef(In_spotId);
	// already exist->del
	if (findIcon != nullptr)
	{
		DelInvasionPortal(In_spotId);
	}
	EGsMapIconType iconType = EGsMapIconType::InvasionPortal;
	const FGsSchemaMinimapIconData* findTbl = FindMapIconData(iconType);
	if (nullptr == findTbl)
	{
		return;
	}

	int depth = findTbl->_depth;
	float updateTime = findTbl->_updateTime;

	UGsUIIconBase* iconWidget = CreateMapIcon(iconType);

	UGsMinimapIconInvasionPortal* makeIcon = NewObject<UGsMinimapIconInvasionPortal>();

	UGsMinimapSceneImage* sceneImage = GSMinimap()->GetSceneImage();


	makeIcon->Initialize(
		iconWidget,
		iconType,
		updateTime,
		sceneImage,
		depth, In_pos);

	// ui의 icon root에 붙이기
	GMessage()->GetUIMinimapIcon().SendMessage(MessageContentMinimap::ATTACH_ICON, makeIcon);

	makeIcon->UpdatePos();

	_listSpawnIcons.Add(makeIcon);

	_mapInvasionPortalIcons.Add(In_spotId, makeIcon);
}
void UGsMinimapIconHandler::DelInvasionPortal(int64 In_spotId)
{
	UGsMinimapIconBase* findIcon = _mapInvasionPortalIcons.FindRef(In_spotId);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapInvasionPortalIcons.Remove(In_spotId);
}

// clear all invasion portal
void UGsMinimapIconHandler::ClearInvasionPortal()
{
	int iconNum = _mapInvasionPortalIcons.Num();
	if (iconNum > 0)
	{
		for (auto& iter : _mapInvasionPortalIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapInvasionPortalIcons.Empty();
	}
}

void UGsMinimapIconHandler::OnUpdateInvasionPortalActive(const IGsMessageParam* InParam)
{	
	const FGsUIMsgParamBool* param = InParam->Cast<const FGsUIMsgParamBool>();
	if (nullptr == param) return;

	if (param->_data == false)
	{
		int32 currentLevelId = 0;
		if (UGsLevelManager* level = GLevel())
		{
			currentLevelId = level->GetCurrentLevelId();
		}

		if (false == GGameData()->IsInvadeWorld())
		{
			if (auto invadeMapdata = GSInterServer()->GetInvadeMapData(currentLevelId).Pin())
			{
				for (auto& spot : invadeMapdata->_spawnPortals)
				{
					DelInvasionPortal(spot.id);
				}
			}
		}
	}
}
void UGsMinimapIconHandler::OnUpdateInvasionPortalSpawnInfo(const IGsMessageParam* InParam)
{
	MakeIconInvasionPortal();
}

void UGsMinimapIconHandler::MakeAutoMovePathIcon(int In_index, FVector2D In_pos, float In_deleteDist)
{
	UGsMinimapIconBase* findIcon = _mapAutoMovePathIcons.FindRef(In_index);
	// already exist->del
	if (findIcon != nullptr)
	{
		DelAutoMovePathIcon(In_index);
	}
	EGsMapIconType iconType = EGsMapIconType::AutoMovePath;
	const FGsSchemaMinimapIconData* findTbl = FindMapIconData(iconType);
	if (nullptr == findTbl)
	{
		return;
	}

	int depth = findTbl->_depth;
	float updateTime = findTbl->_updateTime;

	UGsUIIconBase* iconWidget = CreateMapIcon(iconType);

	UGsMinimapIconAutoMovePath* makeIcon = NewObject<UGsMinimapIconAutoMovePath>();

	UGsMinimapSceneImage* sceneImage = GSMinimap()->GetSceneImage();


	makeIcon->Initialize(
		iconWidget,
		iconType,
		updateTime,
		sceneImage,
		depth, In_pos,
		In_deleteDist);

	// ui의 icon root에 붙이기
	GMessage()->GetUIMinimapIcon().SendMessage(MessageContentMinimap::ATTACH_ICON, makeIcon);

	makeIcon->UpdatePos();

	_listSpawnIcons.Add(makeIcon);

	_mapAutoMovePathIcons.Add(In_index, makeIcon);
}
void UGsMinimapIconHandler::DelAutoMovePathIcon(int In_index)
{
	UGsMinimapIconBase* findIcon = _mapAutoMovePathIcons.FindRef(In_index);

	if (nullptr == findIcon)
	{
		return;
	}

	ReleaseIcon(findIcon);

	findIcon->Finalize();
	_listSpawnIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapAutoMovePathIcons.Remove(In_index);
}

void UGsMinimapIconHandler::AddAllAutoMovePath(TArray<FVector> In_arrayPos)
{
	// 기존꺼 있으면 다 삭제
	if(_mapAutoMovePathIcons.Num() != 0)
	{
		ClearAutoMovePathIcon();
	}

	//  가공한다(원데이터는 꺾이는 지점만 있으므로 더 추가함)
	TArray<FVector2D> arrayMakedPos = MakeAddAutoMovePath(In_arrayPos);

	if (arrayMakedPos.Num() == 0)
	{
		return;
	}

	float minimapAutoPathRemoveDist = GData()->GetGlobalData()->_minimapAutoPathRemoveDist;

	
	for (int i =0 ; arrayMakedPos.Num() > i ; ++i)
	{
		MakeAutoMovePathIcon(i, (FVector2D)arrayMakedPos[i], minimapAutoPathRemoveDist);
	}

	_maxAutoMovePathIndex = _mapAutoMovePathIcons.Num() -1;
}
// 자동 이동 경로 아이콘 삭제
void UGsMinimapIconHandler::ClearAutoMovePathIcon()
{
	int iconNum = _mapAutoMovePathIcons.Num();
	if (iconNum > 0)
	{
		for (auto& iter : _mapAutoMovePathIcons)
		{
			if (iter.Value != nullptr)
			{
				ReleaseIcon(iter.Value);
				iter.Value->Finalize();
				_listSpawnIcons.RemoveSwap(iter.Value);
				iter.Value = nullptr;
			}
		}
		_mapAutoMovePathIcons.Empty();
	}

	_currentAutoMovePathIndex = 0;
}

// 자동 이동 경로 아이콘 정보
void UGsMinimapIconHandler::OnAutoMovePathIconInfo(const IGsMessageParam* InParam)
{
	const FGsMinimapAutoMovePathParam* paramTarget = InParam->Cast<const FGsMinimapAutoMovePathParam>();

	if (paramTarget->_isAdd)
	{
		AddAllAutoMovePath(paramTarget->_arrayPos);
	}
	else
	{
		ClearAutoMovePathIcon();
	}
}
// auto move state 종료
void UGsMinimapIconHandler::OnLocalAutoMoveStop(const IGsMessageParam*)
{
	ClearAutoMovePathIcon();
}

// 경로 path 추가로 만들기
TArray<FVector2D> UGsMinimapIconHandler::MakeAddAutoMovePath(TArray<FVector> In_arrayPos)
{
	TArray<FVector2D> arrayMakePos;

	// 인풋이 없다면 return
	if (In_arrayPos.Num() == 0)
	{
		return arrayMakePos;
	}
	// 크기가 1개면 그거만 넣어서 return;
	if (In_arrayPos.Num() == 1)
	{
		arrayMakePos.Add((FVector2D)In_arrayPos[0]);
		return arrayMakePos;
	}

	// 시작과 끝 사이를 늘려야 하므로
	// 마지막 - 1 되는 위치를 잡아야 하므로(페어로)
	// 1개 적은 위치까지만 체크
	int checkSize = In_arrayPos.Num() -1;

	if (GData() == nullptr)
	{
		return arrayMakePos;
	}

	FVector2D startPos;
	FVector2D endPos;

	// 추가될 거리값
	float minimapAddAutoPathDist = GData()->GetGlobalData()->_minimapAddAutoPathDist;

	// 최소 10은 되어야 함
	minimapAddAutoPathDist = FMath::Max<float>(minimapAddAutoPathDist , 10.0f);

	FVector2D lastPos = (FVector2D)In_arrayPos[0];
	for (int i = 0; i < checkSize; ++i)
	{		
		startPos = (FVector2D)In_arrayPos[i];
		endPos = (FVector2D)In_arrayPos[i+1];

		// 1. 이전 마지막위치와 너무 차이가 날수 있어서 또 메꾸어 본다...
		AddDistMovePath(lastPos, startPos, minimapAddAutoPathDist, arrayMakePos, lastPos);
		
		// 2. 처음꺼 넣기
		// 이거 넣으면 너무 붙어서 나올수도 있어서
		// 이전 위치와 최소 거리가 안되면 추가 안함
		float distPreIndex = FVector2D::Distance(startPos, lastPos);
		if(distPreIndex >= minimapAddAutoPathDist)
		{ 
			arrayMakePos.Add(startPos);
			// 실제 채워진 마지막 위치
			lastPos = startPos;
		}		
	
		// 3. 노드 시작과 끝 사이 채워 놓기
		AddDistMovePath(startPos, endPos, minimapAddAutoPathDist, arrayMakePos, lastPos);
	}
	// 3. 마지막꺼 마지막에만 넣기
	arrayMakePos.Add(endPos);
	return arrayMakePos;
}

// 특정 거리만큼 띄워서 거리 채움
void  UGsMinimapIconHandler::AddDistMovePath(FVector2D In_startPos, FVector2D In_endPos, float In_minimapAddAutoPathDist,
	TArray<FVector2D>& Out_arrayTotalPath, FVector2D& Out_lastPos) 
{
	// 1. 처음과 마지막 위치 사이 채워넣기

	// 2. 시작과 끝 거리 체크
	float dist = FVector2D::Distance(In_startPos, In_endPos);

	// 2.2: 거리값으로 나눈값->몇 번 채울지
	int addCount = (int)(dist / In_minimapAddAutoPathDist);

	FVector2D moveRot = In_endPos - In_startPos;

	moveRot.Normalize();

	FVector2D lastPos = In_startPos;
	for (int j = 0; j < addCount; ++j)
	{
		// 3: 다음 위치 만들기
		lastPos = lastPos + In_minimapAddAutoPathDist * moveRot;
		// 4: 추가
		Out_arrayTotalPath.Add(lastPos);

		Out_lastPos = lastPos;
	}
}

// 이미 지나간 경로이거나 도착하면 삭제하는 처리
void UGsMinimapIconHandler::InvalidateAutoMovePath()
{
	// 하나도 없으면 처리할 필요 없음
	if (_mapAutoMovePathIcons.Num() == 0 ||
		nullptr == _iconLocal)
	{
		return;
	}
	FVector2D localPos = _iconLocal->GetPos();

	TArray<int> arrayRemoveIconIndex;

	for (int i = _currentAutoMovePathIndex ; _maxAutoMovePathIndex >= i; ++i)
	{
		UGsMinimapIconBase* findIcon = _mapAutoMovePathIcons.FindRef(i);
		if (nullptr == findIcon)
		{
			continue;
		}

		UGsMinimapIconAutoMovePath* pathIcon
			= static_cast<UGsMinimapIconAutoMovePath*>(findIcon);
		if (pathIcon == nullptr)
		{
			continue;
		}

		bool isDelete = pathIcon->IsDeleteIcon(localPos);
		// 삭제가 아니면 그만
		if (isDelete == false)
		{
			break;
		}
		else
		{
			arrayRemoveIconIndex.Add(i);
			_currentAutoMovePathIndex++;
		}
	}



	for (auto iter: arrayRemoveIconIndex)
	{
		DelAutoMovePathIcon(iter);
	}
}