#include "GsUIRegionIconRoot.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Boss/GsFieldBossHandler.h"

#include "Message/GSMessageContents.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGame/GsSanctumManager.h"

#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Map/WorldMap/GsSpaceCrackPortalIconInfo.h"
#include "Map/WorldMap/GsRegionInvadeIconInfo.h"
#include "Map/Bookmark/GsBookmarkData.h"
#include "Map/GsMapFunc.h"

#include "Party/GsPartyMemberInfo.h"

#include "SpaceCrack/GsSpaceCrackData.h"

#include "Sanctum/GsSanctumData.h"

#include "GameMode/GsGameModeWorld.h"
#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconBase.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconObject.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconOther.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconLocal.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconParty.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconFixed.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSpot.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelect.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelectMonster.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconFieldBoss.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsUIIconRegionSelectButton.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconTreasureMonster.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconInvasionPortal.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconInvasionNPC.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelectSanctum.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsUIIconRegionSelectButtonSanctum.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionIconMenu.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionTouchMenu.h"
#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIContent/Common/Icon/GsUIIconRegionMapInvasion.h"

#include "DataSchema/Icon/GsSchemaRegionMapIconData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"

#include "Party/GsPartyMemberInfo.h"

#include "UTIL/GsTableUtil.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Boss/GsBossHandler.h"



// 서버 스폰 아이콘 만들기
UGsRegionMapIconObject* UGsUIRegionIconRoot::SpawnIconServer(UGsGameObjectBase* In_target, EGsRegionMapIconType In_iconType)
{
	if (In_target == nullptr)
	{
		return nullptr;
	}

	int64 gameId = In_target->GetGameId();

	UGsRegionMapIconBase* findIcon = _mapServerSpawnIcons.FindRef(gameId);

	if (nullptr != findIcon)
	{
		DespawnIconServer(gameId);
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	UGsRegionMapIconObject* objIcon = nullptr;

	if (In_iconType == EGsRegionMapIconType::LocalPlayer)
	{
		objIcon = NewObject<UGsRegionMapIconLocal>();
	}
	else
	{
		objIcon = NewObject<UGsRegionMapIconOther>();
		objIcon->SetTimmer(data->_updateTime);
	}
	objIcon->SetTargetObject(In_target);

	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType,
		(In_iconType == EGsRegionMapIconType::LocalPlayer)? false : true);

	if (makeIcon == nullptr)
	{
		return nullptr;
	}

	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	

	if (iconSlot != nullptr)
	{
		objIcon->SetIcon(makeIcon, iconSlot ,data->_depth);
	}

	_mapServerSpawnIcons.Add(gameId, objIcon);

	_listRegionIcons.Add(objIcon);

	objIcon->UpdatePos();

	return objIcon;
}

// 서버 스폰 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconServer(int64 In_gameId)
{
	UGsRegionMapIconBase* findIcon = _mapServerSpawnIcons.FindRef(In_gameId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());	

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapServerSpawnIcons.Remove(In_gameId);
}

UGsUIIconBase* UGsUIRegionIconRoot::CreateMapIcon(EGsRegionMapIconType In_type, bool In_useVisibleCheck)
{
	const FGsSchemaRegionMapIconData* tableData =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_type);

	if (tableData != nullptr)
	{
		UGsUIIconBase* iconWidget = FGsIconFactory::CreateIcon(tableData->_iconShapeType);
		if (iconWidget)
		{
			if (tableData->_iconImagePath.IsValid())
			{
				iconWidget->SetIconImage(tableData->_iconImagePath);
			}

			iconWidget->SetIconImageColor(tableData->_iconColor);

			if (true == In_useVisibleCheck)
			{
				// server option visible check
				bool isVisible = FGsWorldMapFunc::IsRegionMapIconTypeVisible(In_type);
				// 몬스터 드랍 필터 켜져있으면 
				// 특정 타입들은 visible 꺼야함
				if (isVisible == true)
				{
					bool isMonsterDropOn = FGsWorldMapFunc::IsMonsterDropOn(In_type);
					if (isMonsterDropOn == true)
					{
						isVisible = false;
					}
				}
				ESlateVisibility newVisibility = (isVisible == true) ?
					ESlateVisibility::SelfHitTestInvisible :
					ESlateVisibility::Hidden;

				iconWidget->SetVisibility(newVisibility);
			}
		}

		return iconWidget;
	}

	return nullptr;
}

void UGsUIRegionIconRoot::NativeConstruct()
{
	Super::NativeConstruct();	
	InitializeMessage();	

	HideIconMenu();
	HideTouchMenu();
}

UGsGameObjectBase* UGsUIRegionIconRoot::GetLocal()
{
	if (_localObject == nullptr)
	{
		_localObject = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	}
	return _localObject;

}


void UGsUIRegionIconRoot::NativeDestruct()
{
	_localObject = nullptr;
	FinalizeMessage();
	ClearAllIcon();
	Super::NativeDestruct();
}

void UGsUIRegionIconRoot::OnSpawnServerIcon(const IGsMessageParam* In_param)
{

	const FGsGameObjectMessageParamRegionMapIcon* paramTarget =
		In_param->Cast<const FGsGameObjectMessageParamRegionMapIcon>();

	if (nullptr == paramTarget ||
		nullptr == paramTarget->_paramOwner)
	{
		return;
	}

	int64 gameId = paramTarget->_paramOwner->GetGameId();
	// 파티 데이터 있는지 체크
	FGsPartyMemberInfo* info = GSParty()->FindPartyMemberInfoByGameId(gameId);

	if (info == nullptr)
	{
		return;
	}

	// 보여지는 맵과 같은 맵인지 체크
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	if (info->GetMapId() != showRegionMapId)
	{
		// 다르면 처리할 필요 없음
		return;
	}
	SpawnIconServer(paramTarget->_paramOwner, paramTarget->_iconType);

	// 기존 생성 되었던 파티용 아이콘 삭제
	DespawnIconParty(gameId);
}
void UGsUIRegionIconRoot::OnDespawnServerIcon(const IGsMessageParam* In_param)
{

	const FGsGameObjectMessageParamRegionMapIcon* paramTarget =
		In_param->Cast<const FGsGameObjectMessageParamRegionMapIcon>();

	int64 gameId = paramTarget->_gameId;
	if (nullptr == paramTarget)
	{
		return;
	}
	DespawnIconServer(gameId);

	// 해당 파티 정보 있으면 파티 아이콘 생성
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

		EGsRegionMapIconType iconType = EGsRegionMapIconType::None;
		if (false == FGsWorldMapFunc::GetPartyIconType(gameId, iconType))
		{
			return;
		}
		SpawnIconParty(memberInfo, iconType);
	}
}

void UGsUIRegionIconRoot::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();

	if (msg == nullptr)
	{
		return;
	}

	MGameObject& msgObject = msg->GetGameObject();

	_arrIconDelegate.Emplace( msgObject.AddUObject(
		MessageGameObject::REGIONMAP_SPAWN_SERVER_ICON, this, &UGsUIRegionIconRoot::OnSpawnServerIcon
	));
	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::REGIONMAP_DESPAWN_SERVER_ICON, this, &UGsUIRegionIconRoot::OnDespawnServerIcon
	));

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_PARTY_MEMBER, this, &UGsUIRegionIconRoot::OnPartyMember
	));

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_SPACE_CRACK_PORTAL, this, &UGsUIRegionIconRoot::OnSpaceCrackPortal
	));

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::REGIONMAP_SPAWN_TREASUREMONSTER_ICON, this, &UGsUIRegionIconRoot::OnTreasureMonsterSpawn
	));
	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::REGIONMAP_UPDATE_TREASUREMONSTER_ICON, this, &UGsUIRegionIconRoot::OnUpdateTreasureMonsterInfo
	));

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::REGIONMAP_SPAWN_INVASIONPORTAL_ICON, this, &UGsUIRegionIconRoot::OnInvasioinPortalSpawn
	));

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_INVASION_NPC, this, &UGsUIRegionIconRoot::OnInvasionNPC
	));

	_arrIconDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_MONSTER_DROP_ICON, this, &UGsUIRegionIconRoot::OnMonsterDropSpawn
	));

	MBoss& msgBoss = msg->GetBoss();
	
	_msgBossHandleList.Emplace(msgBoss.AddUObject(MessageBoss::ADD_FIELD_BOSS_INFO,
		this, &UGsUIRegionIconRoot::OnAddFieldBossInfo));
	_msgBossHandleList.Emplace(msgBoss.AddUObject(MessageBoss::DEL_FIELD_BOSS_INFO,
		this, &UGsUIRegionIconRoot::OnDelFieldBossInfo));

	MUI& msgUI = msg->GetUI();

	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::CHANGE_REGION_ICON_FILTERING,
		this, &UGsUIRegionIconRoot::OnChangeRegionIconFiltering));

	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::INVADE_PORTAL_SPAWNSPOT_UPDATE,
		this, &UGsUIRegionIconRoot::OnUpdateInvasionPortalSpawnInfo));

	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::INVADE_INVASIONTIME_UPDATE,
		this, &UGsUIRegionIconRoot::OnUpdateInvasionPortalActive));

	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::UPDATED_SANCTUM_INFO,
		this, &UGsUIRegionIconRoot::OnUpdatedSanctumInfoUpdated));
}
void UGsUIRegionIconRoot::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();

	if (msg == nullptr)
	{
		return;
	}

	if (_arrIconDelegate.Num() != 0)
	{
		for (auto iter : _arrIconDelegate)
		{
			msg->GetGameObject().Remove(iter);
		}
		_arrIconDelegate.Empty();
	}


	if (_msgBossHandleList.Num() != 0)
	{
		for (auto iter : _msgBossHandleList)
		{
			msg->GetBoss().Remove(iter);
		}
		_msgBossHandleList.Empty();
	}

	if (_msgUIHandlerList.Num() != 0)
	{
		for (auto iter: _msgUIHandlerList)
		{
			msg->GetUI().Remove(iter);
		}
		_msgUIHandlerList.Empty();
	}
}

// 모든 아이콘 생성
void UGsUIRegionIconRoot::MakeAllIcon()
{
	FGsGameDataManager* dataManager = GGameData();
	if (dataManager == nullptr)
	{
		return;
	}
	

	bool isInterServer = dataManager->IsInvadeWorld();

	FGsWorldMapManager* worldmapManager = GSWorldMap();
	if (worldmapManager == nullptr)
	{
		return;
	}


	// 나 만들기	
	// 현재 맵에 있으면 출력
	if (worldmapManager->IsLocalInShowRegionMap() == true)
	{
		UGsGameObjectBase* local = GetLocal();
		if (UGsRegionMapIconObject* makeIcon = SpawnIconServer(local, EGsRegionMapIconType::LocalPlayer))
		{
			UGsRegionMapIconLocal* localIcon = Cast<UGsRegionMapIconLocal>(makeIcon);
			if (localIcon != nullptr)
			{
				localIcon->UpdateRotation();
			}
		}

		_localGameId = local->GetGameId();
	}
	// 파티 만들기
	MakeIconParty();

	//침공전시 워프 스팟, 텔레포트 스팟, 기억 장소 기능, 시공의 틈새 사용 불가
	// 침공전 넘어가서 포탈만 가능하게 요청옴
	// https://jira.com2us.com/jira/browse/CHR-23653
	// 클라이언트 스폰 만들기
	MakeIconClientSpawn();
	
	
	// 목적지 아이콘 만들기
	MakeIconAutoMovePos();

	//침공전시 워프 스팟, 텔레포트 스팟, 기억 장소 기능, 시공의 틈새 사용 불가
	if (false == isInterServer)
	{
		// 즐겨찾기 아이콘 만들기
		MakeIconBookmark();

		// 시공의 틈 포탈 아이콘 만들기
		MakeIconSpaceCrackPortal();
	}
	
	// 필드 보스 아이콘 만들기
	MakeIconFieldBoss();

	// 파티 보물 몬스터 아이콘 만들기
	MakeIconTreasureMonster();

	MakeIconInvasionPortal();

	// check condition
	// 1. current map
	// 2. invasion possible map

	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	int currentLevelId = GLevel()->GetCurrentLevelId();

	if (showRegionMapId == currentLevelId)
	{
		TWeakPtr<FGsInvadeMap> invadeMap = GSInterServer()->GetInvadeMapData(showRegionMapId);
		if (true == invadeMap.IsValid())
		{
			MakeIconInvasionNPC();
		}
	}
	// 성소 아이콘 만들기
	MakeIconSanctum();


	HideIconMenu();
	HideTouchMenu();
}
// 모든 아이콘 삭제
void UGsUIRegionIconRoot::ClearAllIcon()
{
	if (_listRegionIcons.Num() > 0)
	{
		for (auto& iter : _listRegionIcons)
		{
			if (iter != nullptr)
			{
				FGsIconFactory::ReleaseIcon(iter->GetIcon());
				iter->Finalize();
				
				iter = nullptr;
			}
		}
		_listRegionIcons.Empty();
	}

	if (_mapServerSpawnIcons.Num() > 0)
	{
		_mapServerSpawnIcons.Empty();
	}
	
	if (_mapPartyIcons.Num() > 0)
	{
		_mapPartyIcons.Empty();
	}

	if (_mapPortalIcons.Num() > 0)
	{
		_mapPortalIcons.Empty();
	}

	if (_mapMonsterDropIcons.Num() > 0)
	{
		_mapMonsterDropIcons.Empty();
	}

	if (_mapTeleportIcons.Num() > 0)
	{
		_mapTeleportIcons.Empty();
	}
	
	if (_mapBookmarkIcons.Num() > 0)
	{
		_mapBookmarkIcons.Empty();
	}

	if (_mapSpaceCrackPortalIcons.Num() > 0)
	{
		_mapSpaceCrackPortalIcons.Empty();
	}

	if (_mapFieldBossIcons.Num() > 0)
	{
		_mapFieldBossIcons.Empty();
	}

	if (_mapInvadeNPCIcons.Num() > 0)
	{
		_mapInvadeNPCIcons.Empty();
	}

	if (_mapSanctumIcons.Num() > 0)
	{
		_mapSanctumIcons.Empty();
	}

	_mapPartyTreasureMonsterIcons.Reset();
	_mapInvasionPortalIcons.Reset();

	_autoMovePosIcon = nullptr;
}
void UGsUIRegionIconRoot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UGsGameObjectBase* local = GetLocal();
	if (local == nullptr)
	{
		return;
	}

	bool isLocalMoved = false;

	FVector nowPos = local->GetLocation();
	isLocalMoved = !nowPos.Equals(_oldLocalPos);
	if (isLocalMoved)
	{
		_oldLocalPos = nowPos;
	}

	for (TPair<int64, UGsRegionMapIconBase*>& iter: _mapServerSpawnIcons)
	{
		if (iter.Value != nullptr)
		{
			if (isLocalMoved)
			{
				iter.Value->SetLocalPosDirty(true);
			}
			iter.Value->Tick(InDeltaTime);			
		}
	}
	for (TPair<int64, UGsRegionMapIconBase*>& iter : _mapPartyIcons)
	{
		if (iter.Value != nullptr)
		{
			iter.Value->Tick(InDeltaTime);
		}
	}

	// 필드 보스 틱 처리(타이머)
	for (TPair<CreatureId, UGsRegionMapIconFieldBoss*>& iter: _mapFieldBossIcons)
	{
		if (iter.Value != nullptr)
		{
			iter.Value->Tick(InDeltaTime);
		}
	}

	for (auto& e : _mapInvasionPortalIcons)
	{
		if (e.Value) e.Value->Tick(InDeltaTime);
	}

	for (TPair<EGsRegionInvadeNPCType, UGsRegionMapIconBase*>& iter: _mapInvadeNPCIcons)
	{
		if (iter.Value != nullptr)
		{
			iter.Value->Tick(InDeltaTime);
		}
	}
}

// 파티 아이콘 만들기(맵 열릴때 모두 갱신)
void UGsUIRegionIconRoot::MakeIconParty()
{
	// 나
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}

	// 파티에 속했는가
	if (GSParty()->GetIsInParty() == false)
	{
		return;
	}
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	// 파티원들
	const TArray<FGsPartyMemberInfo* > arrPartyMemberInfo = GSParty()->GetAllPartyMemberInfo();

	int64 localGameId = local->GetGameId();

	// check party member world id
	WorldId localNowMapWorldId = GGameData()->GetLoadWorldId();

	for (FGsPartyMemberInfo* info : arrPartyMemberInfo)
	{
		if (info == nullptr)
		{
			continue;
		}
		int64 partyMemberGameId = info->GetGameId();
		// 나는 제외
		if (partyMemberGameId == localGameId)
		{
			continue;
		}

		// 현재 보여지는 맵에 있는지 체크			
		if (info->GetMapId() != showRegionMapId)
		{
			continue;
		}

		// check world id
		WorldId memberWorldId = info->GetWorldId();
		if (memberWorldId != 0 &&
			localNowMapWorldId != memberWorldId)
		{
			continue;
		}

		EGsRegionMapIconType iconType = EGsRegionMapIconType::None;		
		if (false == FGsWorldMapFunc::GetPartyIconType(partyMemberGameId, iconType))
		{
			continue;
		}

		// gameobject에 있으면 object 타입 아이콘으로 만든다
		UGsGameObjectBase* findObj =
			GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, partyMemberGameId);

		// 서버스폰(근거리에 있음)되어있으면 sever icon(game object 위치)
		if (findObj != nullptr)
		{
			SpawnIconServer(findObj, iconType);
		}
		// 그외는 파티 정보 기반 스폰
		else
		{
			SpawnIconParty(info, iconType);			
		}
		
	}

}
// 클라이언트 스폰 아이콘 만들기
void UGsUIRegionIconRoot::MakeIconClientSpawn()
{
	FGsWorldMapManager* worldmapManager = GSWorldMap();
	if (worldmapManager == nullptr)
	{
		return;
	}

	FGsRegionInfo* regionInfo =
		worldmapManager->GetCurrentRegionInfo();

	if (regionInfo == nullptr)
	{
		return;
	}

	// 침공전 넘어가서도 포털만 표시 요청
	// https://jira.com2us.com/jira/browse/CHR-23653
	FGsGameDataManager* dataManager = GGameData();
	if (dataManager == nullptr)
	{
		return;
	}
	bool isInvadeWorld = dataManager->IsInvadeWorld();

	if(isInvadeWorld == false)
	{ 
		// 포털 아이콘 만들기
		if (regionInfo->_arrayPortalInfo.Num() != 0)
		{
			FGsPortalInfo* portalInfo;
			for (int i=0; i< regionInfo->_arrayPortalInfo.Num(); ++i)
			{
				portalInfo = regionInfo->_arrayPortalInfo[i];
				if (portalInfo == nullptr)
				{
					continue;
				}
				SpawnIconPortal(portalInfo->_portalSpotId,
					portalInfo->_portalPos, 
					(portalInfo->_isInTown == true)? EGsRegionMapIconType::Portal_In_Town :EGsRegionMapIconType::Portal,
					portalInfo->_portalName,
					i);
			}
		}
	}
	else
	{
		// 포털 아이콘 만들기(마을 제외)
		if (regionInfo->_arrayInvasionEnterPortalInfo.Num() != 0)
		{
			FGsPortalInfo* portalInfo;
			for (int i = 0; i < regionInfo->_arrayInvasionEnterPortalInfo.Num(); ++i)
			{
				portalInfo = regionInfo->_arrayInvasionEnterPortalInfo[i];
				if (portalInfo == nullptr)
				{
					continue;
				}
				SpawnIconPortal(portalInfo->_portalSpotId,
					portalInfo->_portalPos,
					(portalInfo->_isInTown == true) ? EGsRegionMapIconType::Portal_In_Town : EGsRegionMapIconType::Portal,
					portalInfo->_portalName,
					i);
			}
		}
	}


	// 텔레포트 아이콘 만들기
	if (regionInfo->_arrayTeleportInfo.Num() != 0 && isInvadeWorld == false)
	{
		for (auto iter : regionInfo->_arrayTeleportInfo)
		{
			if (iter == nullptr)
			{
				continue;
			}

			SpawnIconTeleport(iter->_teleportSpotId, iter->_teleportPos,
				EGsRegionMapIconType::TeleportSpot,
				iter->_teleportName);
		}
	}

}
// 목적지 아이콘 만들기
void UGsUIRegionIconRoot::MakeIconAutoMovePos()
{
	// 목적지 표시 가능하지 않으면 안함	
	if (false == GSWorldMap()->IsUseableShowAutoMovePos())
	{
		return;
	}

	// 현재맵이랑 다르면 안만듬
	if (false == GSWorldMap()->IsTargetMapSame(EGsWorldMapChangeType::TO_REGIONMAP_AUTOMOVEPOS))
	{
		return;
	}
	FVector pos = GSWorldMap()->GetAutoMovePos();

	SpawnIconAutoMovePos(pos, EGsRegionMapIconType::AutoMoveTargetPos);
}
// 즐겨찾기 아이콘 만들기
void UGsUIRegionIconRoot::MakeIconBookmark()
{
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();

	TArray<FGsBookmarkData*> arrData = GSBookmark()->GetBookmarkData();
	if (arrData.Num() == 0)
	{
		return;
	}
	for (FGsBookmarkData* iter : arrData)
	{
		// 정보가 없거나
		// 맵 id 가 다르면 처리안함
		if (nullptr == iter ||
			iter->GetMapId() != showRegionMapId)
		{
			continue;
		}

		SpawnIconBookmark(iter->GetId(), iter->GetPos(), EGsRegionMapIconType::Bookmark);
	}
}
// 시공의 틈새 포탈 아이콘 만들기
void UGsUIRegionIconRoot::MakeIconSpaceCrackPortal()
{
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();

	// 현재 채널과 다르면 처리 안함
	int currentChannel = GGameData()->GetCurrentChannel();

	if (false == GGameData()->IsInvadeWorld())
	{
		const TMap<MapId, TSharedPtr<FGsSpaceCrackAlarmInfo>>& infos = GSSpaceCrack()->GetSpaceCrackAlarmInfoMap();
		for (auto& iter : infos)
		{
			if (false == iter.Value.IsValid() ||
				iter.Value->_regionMapId != showRegionMapId)
			{
				continue;
			}

			SpawnIconSpaceCrackPortal(iter.Key,
				iter.Value->_pos, EGsRegionMapIconType::SpaceCrackPortal);
		}
	}
}
// 필드 보스 아이콘 만들기
void UGsUIRegionIconRoot::MakeIconFieldBoss()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if ( nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}
	
	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsFieldBossHandler* fieldBossHandler = hud->GetBossHandler()->GetFieldBossHandler();
	if (nullptr == fieldBossHandler)
	{
		return;
	}	

	// 보여지는 맵
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	int currentLevelId = GLevel()->GetCurrentLevelId();

	const TArray<const struct FGsSchemaFieldBossData*>& bossArray = FGsFieldBossHandler::GetFieldBossDataByLevelId(showRegionMapId);	
	if (bossArray.Num() == 0)
	{
		return;
	}
	
	FVector spawnPos = FVector::ZeroVector;
	EGsRegionFieldBossState uiSwitchState;
	FDateTime spawnTime;
	bool isHaveSpawnTime = false;
	FGsFieldBossInfoCooperation* findInfo;
	for (const FGsSchemaFieldBossData* fieldBoss : bossArray)
	{
		if (nullptr == fieldBoss->bossNpcId.GetRow())
		{
			continue;
		}

		if (false == FGsFieldBossHandler::IsNeedToBeShownWorldMap(fieldBoss->bossId))
		{
			continue;
		}
		
		findInfo = fieldBossHandler->FindBossInfoCooperation(fieldBoss->bossId);
		if (nullptr == findInfo)
		{
			continue;
		}

		spawnPos = findInfo->_spawnPos;		
		uiSwitchState = EGsRegionFieldBossState::Spawn;		
		isHaveSpawnTime = false;

		// 현재 맵이랑 지역맵 id랑 같으면 상태 체크
		if (showRegionMapId == currentLevelId)
		{
			isHaveSpawnTime = fieldBossHandler->TryGetFieldBossSpawnTime(fieldBoss->bossId, spawnTime);
		}

		SpawnIconFieldBoss(fieldBoss->bossNpcId.GetRow()->id
		, spawnPos
		, EGsRegionMapIconType::FieldBoss
		, spawnTime, isHaveSpawnTime);
	}	
}

void UGsUIRegionIconRoot::MakeIconTreasureMonster()
{
	TArray<FGsTreasureMonsterMapInfo> monsterInfoList;
	if (GSWorldMap()->GetShowRegionTreasureMonsterInfo(monsterInfoList) > 0)
	{
		for (const auto& monsterInfo : monsterInfoList)
		{
			SpawnIconTreasureMonster(monsterInfo._seperateId, monsterInfo._position,
				monsterInfo._localPlayerOwn ? EGsRegionMapIconType::MyTreasureMonster : EGsRegionMapIconType::TreasureMonster);
		}
	}

	//if(auto localPlayer = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
	//{
	//	if (!localPlayer->IsOwnTreasureMonster())
	//	{
	//		return;
	//	}
	//	const auto& info = localPlayer->GetOwnTreasureMonsterInfo();
	//	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	//	if (showRegionMapId == info._mapId)
	//	{
	//		SpawnIconTreasureMonster(EGsRegionMapIconType::TreasureMonster);
	//		if (_myTreasureMonsterIcon)
	//		{
	//			_myTreasureMonsterIcon->SetLocalPosDirty(true);
	//			_myTreasureMonsterIcon->UpdatePos();
	//		}
	//	}
	//}
}

void UGsUIRegionIconRoot::MakeIconInvasionPortal()
{
	auto mapId = GSWorldMap()->GetShowRegionMapId();

	if (false == GGameData()->IsInvadeWorld())
	{
		if (auto invadeMapdata = GSInterServer()->GetInvadeMapData(mapId).Pin())
		{
			if (invadeMapdata->_activePortal)
			{
				for (auto& spot : invadeMapdata->_spawnPortals)
				{
					SpawnIconInvasionPortal(spot.id, spot.pos);
				}
			}
		}
	}
}

// 파티 정보로 아이콘 만들기(시야내에 파티 없을때)
void UGsUIRegionIconRoot::SpawnIconParty(FGsPartyMemberInfo* In_partyInfo, EGsRegionMapIconType In_iconType)
{
	if (In_partyInfo == nullptr)
	{
		return;
	}

	int64 gameId = In_partyInfo->GetGameId();

	UGsRegionMapIconBase* findIcon = _mapPartyIcons.FindRef(gameId);
	if (findIcon != nullptr)
	{
		// 디스폰 처리
		DespawnIconParty(gameId);
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	UGsRegionMapIconParty* partyIcon = NewObject<UGsRegionMapIconParty>();

	partyIcon->SetPartyMemberInfo(In_partyInfo);
	partyIcon->SetTimmer(data->_updateTime);


	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType, true);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		partyIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}

	_mapPartyIcons.Add(gameId, partyIcon);

	_listRegionIcons.Add(partyIcon);

	partyIcon->UpdatePos();

}
// 파티 정보로 만든 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconParty(int64 In_gameId)
{
	UGsRegionMapIconBase* findIcon = _mapPartyIcons.FindRef(In_gameId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapPartyIcons.Remove(In_gameId);

}

// 스팟 아이콘 만들기(텔레포트)
void UGsUIRegionIconRoot::SpawnIconTeleport(int64 In_spotId, FVector In_pos, EGsRegionMapIconType In_iconType,
	FText In_name)
{
	UGsRegionMapIconBase* findIcon = _mapTeleportIcons.FindRef(In_spotId);
	if (findIcon != nullptr)
	{
		DespawnIconTeleport(In_spotId);
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	UGsRegionMapIconSpot* fixedIcon = NewObject<UGsRegionMapIconSpot>();
	fixedIcon->SetActorPos(FVector2D(In_pos));

	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType, true);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);	

	if (iconSlot != nullptr)
	{
		fixedIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}

	fixedIcon->SetSpotName(In_name);

	_mapTeleportIcons.Add(In_spotId, fixedIcon);

	_listRegionIcons.Add(fixedIcon);

	fixedIcon->UpdatePos();

}
// 클라 스폰 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconTeleport(int64 In_spotId)
{
	UGsRegionMapIconBase* findIcon = _mapTeleportIcons.FindRef(In_spotId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapTeleportIcons.Remove(In_spotId);
}


// 포털 아이콘 만들기
void UGsUIRegionIconRoot::SpawnIconPortal(int64 In_spotId, FVector In_pos, EGsRegionMapIconType In_iconType,
	FText In_name, int In_selectIndex)
{
	UGsRegionMapIconBase* findIcon = _mapPortalIcons.FindRef(In_spotId);
	if (findIcon != nullptr)
	{
		DespawnIconPortal(In_spotId);
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	UGsRegionMapIconSelect* fixedIcon = NewObject<UGsRegionMapIconSelect>();
	fixedIcon->SetActorPos(FVector2D(In_pos));
	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		fixedIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}
	fixedIcon->SetSelect(false);
	fixedIcon->SetSpotName(In_name);
	fixedIcon->SetSelectIndex(In_selectIndex);


	UGsUIIconRegionSelectButton* iconSelectButton = Cast<UGsUIIconRegionSelectButton>(makeIcon);
	if (iconSelectButton != nullptr)
	{
		iconSelectButton->SetClickCallback([this, fixedIcon]
			{
				OnClickPortalIcon(fixedIcon);
			});
	}

	_mapPortalIcons.Add(In_spotId, fixedIcon);

	_listRegionIcons.Add(fixedIcon);

	fixedIcon->UpdatePos();
}

void UGsUIRegionIconRoot::OnClickPortalIcon(UGsRegionMapIconSelect* In_clickedIconObj)
{
	UnselectAllPortal();
	UnselectAllBookmark();
	UnselectAllSanctum();

	//if (In_clickedIconObj)
	//{
	//	In_clickedIconObj->SetSelect(true);
	//}	

	if (_callbackClickPortalIcon)
	{		
		_callbackClickPortalIcon(In_clickedIconObj);		
	}
}
// 침공전 포털 아이콘 클릭
void UGsUIRegionIconRoot::OnClickInvasionIcon(UGsUIIconRegionMapInvasion* In_invasionIcon, int64 In_spotId, FVector In_pos, FString In_portalName)
{
	// 모든 아이콘 선택 해제
	UnselectAllPortal();
	UnselectAllBookmark();
	UnselectAllSanctum();

	if (_callbackClickInvasionPortalIcon)
	{
		_callbackClickInvasionPortalIcon(In_invasionIcon, In_spotId, In_pos, In_portalName);
	}

}
void UGsUIRegionIconRoot::OnClickMonsterDropIcon(UGsRegionMapIconSelectMonster* In_clickedIconObj, FVector In_pos)
{
	if (_callbackClickMonsterDropIcon != nullptr)
	{
		_callbackClickMonsterDropIcon(In_clickedIconObj, In_pos);
	}
}

void UGsUIRegionIconRoot::OnClickMoveableTerrain(FVector In_pos)
{
	FVector2D uiPos = GSWorldMap()->GetUIPos(FVector2D(In_pos));

	UGsUIRegionTouchMenu* findMenu = GetNowTouchMenu(uiPos);
	if(findMenu != nullptr)
	{ 
		findMenu->SetRenderTranslation(uiPos);

		// save move 3d pos
		findMenu->SetData(In_pos);
		findMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
// show icon menu
void UGsUIRegionIconRoot::ShowIconMenu(UGsRegionMapIconSelect* In_clickedIconObj)
{
	if (In_clickedIconObj == nullptr)
	{
		return;
	}

	// icon pos
	UGsUIIconBase* icon = In_clickedIconObj->GetIcon();
	if (icon == nullptr)
	{
		return;
	}

	// 클릭한 아이콘 위에 메뉴 띄우기
	FVector2D pos = icon->RenderTransform.Translation;

	UGsUIRegionIconMenu* findMenu = GetNowIconMenu(pos);
	if (findMenu != nullptr)
	{
		findMenu->SetRenderTranslation(pos);

		// select index 
		// 데이터 세팅
		int selectIndex = In_clickedIconObj->GetListSelectIndex();
		findMenu->SetNormalPortalData(selectIndex);
		// menu visible 
		findMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}


	// hide touch menu
	// 땅찍고 이동 끄기
	HideTouchMenu();
}
// 아이콘 메뉴 출력(침공 포탈)
void UGsUIRegionIconRoot::ShowInvasionIconMenu(UGsUIIconRegionMapInvasion* In_icon, int64 In_spotId, FVector In_pos, FString In_text)
{
	if (In_icon == nullptr)
	{
		return;
	}

	FVector2D pos = In_icon->RenderTransform.Translation;

	UGsUIRegionIconMenu* findMenu = GetNowIconMenu(pos);
	if (findMenu != nullptr)
	{
		findMenu->SetRenderTranslation(pos);

		// 침공 데이터 세팅
		findMenu->SetInvasionPortalData(In_spotId, In_pos, In_text);

		// menu visible 
		findMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}


	// hide touch menu
	HideTouchMenu();
}
// hide icon menu
void UGsUIRegionIconRoot::HideIconMenu()
{
	_iconMenu->SetVisibility(ESlateVisibility::Hidden);
	_iconMenuDown->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIRegionIconRoot::ShowIconMenuByPortalIndex(int64 inIndex)
{
	UnselectAllPortal();
	UnselectAllBookmark();
	UnselectAllSanctum();

	for(TPair<int64, UGsRegionMapIconSelect*>& iconInfo : _mapPortalIcons)
	{		
		if (iconInfo.Value)
		{
			if (iconInfo.Value->GetListSelectIndex() == inIndex)
			{
				ShowIconMenu(iconInfo.Value);			
				iconInfo.Value->SetSelect(true);
				return;
			}			
		}		
	}	
}

void UGsUIRegionIconRoot::ShowIconMenuByBookmarkIndex(int64 inIndex)
{
	UnselectAllPortal();
	UnselectAllBookmark();
	UnselectAllSanctum();

	for (TPair<uint64, UGsRegionMapIconSelect*>& iconInfo : _mapBookmarkIcons)
	{
		if (iconInfo.Value)
		{
			if (iconInfo.Value->GetListSelectIndex() == inIndex)
			{
				iconInfo.Value->SetSelect(true);
				HideIconMenu();
				return;
			}
		}
	}
}

void UGsUIRegionIconRoot::HideTouchMenu()
{
	_touchMenu->SetVisibility(ESlateVisibility::Hidden);
	_touchMenuDown->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIRegionIconRoot::UnselectAllPortal()
{
	for (TPair<int64, UGsRegionMapIconSelect*>& icon : _mapPortalIcons)
	{
		icon.Value->SetSelect(false);
	}
}

void UGsUIRegionIconRoot::UnselectAllBookmark()
{
	for (TPair<uint64, UGsRegionMapIconSelect*>& icon : _mapBookmarkIcons)
	{
		icon.Value->SetSelect(false);
	}
}
// 성소 모든 아이콘 비선택으로 변경
void UGsUIRegionIconRoot::UnselectAllSanctum()
{
	for (TPair<int32, UGsRegionMapIconBase*>& icon : _mapSanctumIcons)
	{
		UGsRegionMapIconBase* iconBase = icon.Value;
		if (iconBase == nullptr)
		{
			continue;
		}
		UGsRegionMapIconSelectSanctum* iconSanctum = Cast<UGsRegionMapIconSelectSanctum>(iconBase);
		if (iconSanctum == nullptr)
		{
			continue;
		}

		iconSanctum->SetSelect(false);
	}
}
// 포탈 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconPortal(int64 In_spotId)
{
	UGsRegionMapIconBase* findIcon = _mapPortalIcons.FindRef(In_spotId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapPortalIcons.Remove(In_spotId);
}

// 즐겨찾기 아이콘 만들기
void UGsUIRegionIconRoot::SpawnIconBookmark(uint64 In_bookmarkId, FVector In_pos, EGsRegionMapIconType In_iconType)
{
	UGsRegionMapIconBase* findIcon = _mapBookmarkIcons.FindRef(In_bookmarkId);
	if (findIcon != nullptr)
	{
		DespawnIconBookmark(In_bookmarkId);
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	UGsRegionMapIconSelect* fixedIcon = NewObject<UGsRegionMapIconSelect>();
	fixedIcon->SetActorPos(FVector2D(In_pos));
	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType, true);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		fixedIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}
	fixedIcon->SetSelect(false);
	fixedIcon->SetSelectIndex(In_bookmarkId);

	_mapBookmarkIcons.Add(In_bookmarkId, fixedIcon);

	_listRegionIcons.Add(fixedIcon);

	fixedIcon->UpdatePos();
}
// 즐겨찾기 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconBookmark(uint64 In_bookmarkId)
{
	UGsRegionMapIconBase* findIcon = _mapBookmarkIcons.FindRef(In_bookmarkId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapBookmarkIcons.Remove(In_bookmarkId);
}

// 목적지 아이콘 만들기
void UGsUIRegionIconRoot::SpawnIconAutoMovePos(FVector In_pos, EGsRegionMapIconType In_iconType)
{
	if (_autoMovePosIcon != nullptr)
	{
		DespawnIconAutoMovePos();
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	_autoMovePosIcon = NewObject<UGsRegionMapIconSelect>();
	_autoMovePosIcon->SetActorPos(FVector2D(In_pos));

	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		_autoMovePosIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}
	// 여기서 선택 처리 한다
	bool isSel = false;
	if (_isDirtyAutoMovePosSel == true)
	{
		_isDirtyAutoMovePosSel = false;
		isSel = true;
	}
	_autoMovePosIcon->SetSelect(isSel);
	_listRegionIcons.Add(_autoMovePosIcon);

	_autoMovePosIcon->UpdatePos();
}
// 목적지 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconAutoMovePos()
{
	if (_autoMovePosIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(_autoMovePosIcon->GetIcon());

	_autoMovePosIcon->Finalize();
	_listRegionIcons.RemoveSwap(_autoMovePosIcon);

	_autoMovePosIcon = nullptr;
}
// 북마크 아이콘 선택
void UGsUIRegionIconRoot::SelectBookmarkIcon(
	bool In_isSelect, uint64 In_bookmarkId)
{
	for (auto iter: _mapBookmarkIcons)
	{
		if (iter.Value == nullptr)
		{
			continue;
		}
		// 선택이면서
		// id같으면
		if (In_isSelect == true &&
			In_bookmarkId == iter.Key)
		{
			iter.Value->SetSelect(true);
		}
		else
		{
			iter.Value->SetSelect(false);
		}
	}
}
// 포탈 아이콘 선택
void UGsUIRegionIconRoot::SelectPortalIcon(bool In_isSelect, int64 In_spotId)
{
	for (auto portalIter : _mapPortalIcons)
	{
		if (portalIter.Value == nullptr)
		{
			continue;
		}
		// 선택이면서
		// id같으면
		if (In_isSelect == true &&
			In_spotId == portalIter.Key)
		{
			portalIter.Value->SetSelect(true);
		}
		else
		{
			portalIter.Value->SetSelect(false);
		}
	}
}

// 자동 이동 목표 위치 아이콘 선택
void UGsUIRegionIconRoot::SelectAutoMoveTargetPosIcon(bool In_isSelect)
{
	if (_autoMovePosIcon == nullptr)
	{
		return;
	}

	_autoMovePosIcon->SetSelect(In_isSelect);
}

// 몬스터 드랍 목표 위치 아이콘 선택
void UGsUIRegionIconRoot::SelectMonsterDropIcon(bool In_isSelect, int64 In_spotId)
{
	for (auto dropIter : _mapMonsterDropIcons)
	{
		if (dropIter.Value == nullptr)
		{
			continue;
		}
		// 선택이면서
		// id같으면
		if (In_isSelect == true &&
			In_spotId == dropIter.Key)
		{
			dropIter.Value->SetSelect(true);
		}
		else
		{
			dropIter.Value->SetSelect(false);
		}
	}
}

// 아이콘 선택 공용
// 1. 아이콘 타입
// 2. 선택 인지
// 3. 즐겨찾기:uin64, 포탈: int64
void UGsUIRegionIconRoot::SelectIcon(EGsRegionIconSelectType In_type, bool In_isSelect, uint64 In_id)
{
	if (In_type == EGsRegionIconSelectType::PortalIcon)
	{
		int64 spotId = static_cast<int64>(In_id);
		SelectPortalIcon(In_isSelect , spotId);

		// 나머진 끈다
		SelectBookmarkIcon(false);
		SelectAutoMoveTargetPosIcon(false);
		SelectMonsterDropIcon(false);
	}
	else if (In_type == EGsRegionIconSelectType::BookmarkIcon)
	{
		SelectBookmarkIcon(In_isSelect, In_id);

		// 나머진 끈다
		SelectPortalIcon(false);
		SelectAutoMoveTargetPosIcon(false);
		SelectMonsterDropIcon(false);
	}
	else if (In_type == EGsRegionIconSelectType::AutoMoveTargetPosIcon)
	{
		SelectAutoMoveTargetPosIcon(In_isSelect);

		// 나머진 끈다
		SelectPortalIcon(false);
		SelectBookmarkIcon(false);
		SelectMonsterDropIcon(false);
	}
	else
	{
		SelectMonsterDropIcon(In_isSelect, In_id);

		// 나머진 끈다
		SelectPortalIcon(false);
		SelectBookmarkIcon(false);
		SelectAutoMoveTargetPosIcon(false);
	}
}

// 파티 멤버 (추가, 삭제)
void UGsUIRegionIconRoot::OnPartyMember(const IGsMessageParam* In_param)
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
void UGsUIRegionIconRoot::AddPartyMember(const TArray<int64>& In_arrId)
{
	// 보여지는 맵과 같은 맵인지 체크
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();

	for (int64 iter : In_arrId)
	{
		EGsRegionMapIconType iconType = EGsRegionMapIconType::None;
		if (false == FGsWorldMapFunc::GetPartyIconType(iter, iconType))
		{
			continue;
		}

		// gameobject에 있으면 object 타입 아이콘으로 만든다
		UGsGameObjectBase* findObj =
			GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, iter);

		if (findObj != nullptr)
		{
			SpawnIconServer(findObj, iconType);
		}
		else
		{
			// game object에 없으면 info 기준으로 만든다
			// 현재 보는 맵이랑 다르면 처리 안함
			FGsPartyMemberInfo* info = GSParty()->FindPartyMemberInfoByGameId(iter);
			if (nullptr == info ||
				info->GetMapId() != showRegionMapId)
			{
				continue;
			}

			SpawnIconParty(info, iconType);
		}
	}
}
// 파티 멤버 삭제
void UGsUIRegionIconRoot::DelPartyMember(const TArray<int64>& In_arrId)
{
	for (int64 iter : In_arrId)
	{
		// gameobject에 있으면 object 타입 아이콘으로 삭제한다
		UGsGameObjectBase* findObj =
			GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, iter);

		if (findObj != nullptr)
		{
			DespawnIconServer(iter);
		}
		else
		{
			// 아니면 info 기준 삭제
			DespawnIconParty(iter);
		}
	}
}

// 시공의 틈새 포탈(추가, 삭제)
void UGsUIRegionIconRoot::OnSpaceCrackPortal(const IGsMessageParam* In_param)
{
	const FGsMapSpaceCrackAlarmParam* paramTarget =
		In_param->Cast<const FGsMapSpaceCrackAlarmParam>();


	// 보여지는 맵과 같은 맵인지 체크
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	// 현재 보는 맵과 다른맵이면 처리 안함
	if (showRegionMapId != paramTarget->_regionMapId)
	{
		return;
	}

	if (paramTarget->_isAdd == true)
	{
		SpawnIconSpaceCrackPortal(paramTarget->_regionMapId, paramTarget->_pos, EGsRegionMapIconType::SpaceCrackPortal);
	}
	else
	{
		DespawnIconSpaceCrackPortal(paramTarget->_regionMapId);
	}
}

// 시공의 틈새 포탈 아이콘 만들기
void UGsUIRegionIconRoot::SpawnIconSpaceCrackPortal(int64 In_gameId, const FVector& In_pos, EGsRegionMapIconType In_iconType)
{
	UGsRegionMapIconBase* findIcon = _mapSpaceCrackPortalIcons.FindRef(In_gameId);

	if (nullptr != findIcon)
	{
		DespawnIconSpaceCrackPortal(In_gameId);
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	UGsRegionMapIconFixed* fixedIcon = NewObject<UGsRegionMapIconFixed>();
	fixedIcon->SetActorPos(FVector2D(In_pos));

	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType, true);

	if (makeIcon == nullptr)
	{
		return;
	}

	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		fixedIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}

	_mapSpaceCrackPortalIcons.Add(In_gameId, fixedIcon);

	_listRegionIcons.Add(fixedIcon);

	fixedIcon->UpdatePos();
}
// 시공의 틈새 포탈 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconSpaceCrackPortal(int64 In_gameId)
{
	UGsRegionMapIconBase* findIcon = _mapSpaceCrackPortalIcons.FindRef(In_gameId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapSpaceCrackPortalIcons.Remove(In_gameId);
}

// 필드 보스 아이콘 만들기
void UGsUIRegionIconRoot::SpawnIconFieldBoss(CreatureId In_creatureId, const FVector& In_pos, 
	EGsRegionMapIconType In_iconType, 
	FDateTime In_spawnTime,
	bool In_isHaveSpawnTime)
{
	UGsRegionMapIconFieldBoss* findIcon = _mapFieldBossIcons.FindRef(In_creatureId);
	if (findIcon != nullptr)
	{
		DespawnIconFieldBoss(In_creatureId);
	}


	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	UGsRegionMapIconFieldBoss* fieldBoss = NewObject<UGsRegionMapIconFieldBoss>();
	fieldBoss->SetActorPos(FVector2D(In_pos));


	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType, true);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		fieldBoss->SetIcon(makeIcon, iconSlot, data->_depth);
	}

	fieldBoss->SetSpawnTime(In_isHaveSpawnTime, In_spawnTime);
	fieldBoss->UpdateState(true);
	fieldBoss->UpdateRemainTime(true);
	fieldBoss->SetClickCallback([this, In_creatureId]
		{
			OnClickFieldBossIcon(In_creatureId);
		});


	_mapFieldBossIcons.Add(In_creatureId, fieldBoss);

	_listRegionIcons.Add(fieldBoss);

	fieldBoss->UpdatePos();
}
// 필드 보스 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconFieldBoss(CreatureId In_creatureId)
{
	UGsRegionMapIconBase* findIcon = _mapFieldBossIcons.FindRef(In_creatureId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapFieldBossIcons.Remove(In_creatureId);
}

void UGsUIRegionIconRoot::SpawnIconTreasureMonster(int64 In_gameId, const FVector& In_pos, EGsRegionMapIconType In_iconType)
{
	if (_mapPartyTreasureMonsterIcons.FindRef(In_gameId))
	{
		DespawnIconTreasureMonster(In_gameId);
	}

	if (const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType))
	{
		UGsRegionMapIconTreasureMonster* trMonsterIcon = NewObject<UGsRegionMapIconTreasureMonster>();
		trMonsterIcon->SetMonsterSeperateID(In_gameId);

		UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType, true);

		if (makeIcon == nullptr)
		{
			return;
		}

		UCanvasPanelSlot* iconSlot = _iconRoot->AddChildToCanvas(makeIcon);
		if (iconSlot != nullptr)
		{
			trMonsterIcon->SetIcon(makeIcon, iconSlot, data->_depth);
			trMonsterIcon->SetLocalPosDirty(true);
			trMonsterIcon->UpdatePos();
		}

		_mapPartyTreasureMonsterIcons.Add(In_gameId, trMonsterIcon);
		_listRegionIcons.Add(trMonsterIcon);
	}
}

void UGsUIRegionIconRoot::DespawnIconTreasureMonster(int64 In_gameId)
{
	if (auto iter = _mapPartyTreasureMonsterIcons.FindRef(In_gameId))
	{
		FGsIconFactory::ReleaseIcon(iter->GetIcon());

		iter->Finalize();

		_listRegionIcons.RemoveSwap(iter);
		_mapPartyTreasureMonsterIcons.Remove(In_gameId);
	}

}

void UGsUIRegionIconRoot::SpawnIconInvasionPortal(int64 InSpotId, FVector InPos, EGsRegionMapIconType InIconType /*= EGsRegionMapIconType::InvasionPortal*/)
{
	if (_mapInvasionPortalIcons.Find(InSpotId))
	{
		DespawnIconInvasionPortal(InSpotId);
	}

	if (const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(InIconType))
	{
		UGsRegionMapIconInvasionPortal* portalIcon = NewObject<UGsRegionMapIconInvasionPortal>();
		portalIcon->SetActorPos(FVector2D(InPos));

		UGsUIIconBase* makeIcon = CreateMapIcon(InIconType, true);

		if (makeIcon == nullptr)
		{
			return;
		}

		UCanvasPanelSlot* iconSlot = _iconRoot->AddChildToCanvas(makeIcon);
		if (iconSlot != nullptr)
		{
			portalIcon->SetIcon(makeIcon, iconSlot, data->_depth);
		}

		UGsUIIconRegionMapInvasion* iconSelectButton = Cast<UGsUIIconRegionMapInvasion>(makeIcon);
		if (iconSelectButton != nullptr)
		{
			FText findText;
			FText::FindText(TEXT("PropNameText"), TEXT("Prop_90010001"), findText);


			iconSelectButton->SetClickCallback([this, 
			iconSelectButton, InSpotId, InPos, findText]
				{
					OnClickInvasionIcon(iconSelectButton, InSpotId, InPos, findText.ToString());
				});
		}

		_mapInvasionPortalIcons.Add(InSpotId, portalIcon);
		_listRegionIcons.Add(portalIcon);


		auto mapId = GSWorldMap()->GetShowRegionMapId();
		if (auto invadeMapdata = GSInterServer()->GetInvadeMapData(mapId).Pin())
		{
			portalIcon->SetRemainTime(invadeMapdata->GetInvasionEndTime());

			if (invadeMapdata->_enrolledSpot.Find(InSpotId) != INDEX_NONE)
				 portalIcon->SetActivePortal(true);
			else portalIcon->SetActivePortal(false);
			
		}

		portalIcon->UpdatePos();
	}
}

void UGsUIRegionIconRoot::DespawnIconInvasionPortal(int64 InSpotId)
{
	if (auto iter = _mapInvasionPortalIcons.Find(InSpotId))
	{
		FGsIconFactory::ReleaseIcon((*iter)->GetIcon());

		(*iter)->Finalize();
		_listRegionIcons.RemoveSwap((*iter));
		_mapInvasionPortalIcons.Remove(InSpotId);
	}
}

void UGsUIRegionIconRoot::OnFieldBossSpawn(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}
	UGsRegionMapIconFieldBoss* findIcon = _mapFieldBossIcons.FindRef(param->_fieldBossId);
	if (findIcon != nullptr)
	{
		findIcon->SetBossState(EGsRegionFieldBossState::Spawn);
	}
}
void UGsUIRegionIconRoot::OnFieldBossDespawn(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}
	UGsRegionMapIconFieldBoss* findIcon = _mapFieldBossIcons.FindRef(param->_fieldBossId);
	if (findIcon != nullptr)
	{
		findIcon->SetBossState(EGsRegionFieldBossState::NotSpawn);
	}
}
void UGsUIRegionIconRoot::OnFieldBossDie(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	UGsRegionMapIconFieldBoss* findIcon = _mapFieldBossIcons.FindRef(param->_fieldBossId);
	if (findIcon != nullptr)
	{
		findIcon->SetBossState(EGsRegionFieldBossState::NotSpawn);
	}
}

void UGsUIRegionIconRoot::OnAddFieldBossInfo(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	UGsRegionMapIconFieldBoss* findIcon = _mapFieldBossIcons.FindRef(param->_fieldBossId);
	if (findIcon != nullptr)
	{
		FDateTime spawnTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(param->_msgTime);
		findIcon->SetSpawnTime(true, spawnTime);
		// 갱신
		findIcon->UpdateState(true);
		findIcon->UpdateRemainTime(true);
	}
}
void UGsUIRegionIconRoot::OnDelFieldBossInfo(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	UGsRegionMapIconFieldBoss* findIcon = _mapFieldBossIcons.FindRef(param->_fieldBossId);
	if (findIcon != nullptr)
	{
		findIcon->SetBossState(EGsRegionFieldBossState::NotSpawn);
		FDateTime emptyTime;
		findIcon->SetSpawnTime(false, emptyTime);
		
	}
}

void UGsUIRegionIconRoot::OnTreasureMonsterSpawn(const IGsMessageParam* InParam)
{
	const FGsMapTreasureMonsterParam* paramTarget = InParam->Cast<const FGsMapTreasureMonsterParam>();

	if (paramTarget->_isAdd == true)
	{
		auto info = GSWorldMap()->GetTreasureMonsterMapInfo(paramTarget->_seperator, GSWorldMap()->GetShowRegionMapId());
		if (info.IsNoTreasureMonster() == false)
		{
			SpawnIconTreasureMonster(info._seperateId, info._position, 
				info._localPlayerOwn ? EGsRegionMapIconType::MyTreasureMonster : EGsRegionMapIconType::TreasureMonster);
		}
	}
	else
	{
		DespawnIconTreasureMonster(paramTarget->_seperator);
	}
}

void UGsUIRegionIconRoot::OnUpdateTreasureMonsterInfo(const IGsMessageParam* InParam)
{
	const FGsMapTreasureMonsterParam* paramTarget = InParam->Cast<const FGsMapTreasureMonsterParam>();

	if (auto iter = _mapPartyTreasureMonsterIcons.FindRef(paramTarget->_seperator))
	{
		iter->SetLocalPosDirty(true);
		iter->UpdatePos();
	}
}

void UGsUIRegionIconRoot::OnInvasioinPortalSpawn(const IGsMessageParam* InParam)
{
	const FGsMapInvasionPortalParam* paramTarget = InParam->Cast<const FGsMapInvasionPortalParam>();

	if (GSWorldMap()->GetShowRegionMapId() != paramTarget->_mapId)
	{
		return;
	}

	if (paramTarget->_isAdd == true)
	{
		SpawnIconInvasionPortal(paramTarget->_seperator, paramTarget->_pos);
	}
	else
	{
		DespawnIconInvasionPortal(paramTarget->_seperator);
	}
}

void UGsUIRegionIconRoot::OnUpdateInvasionPortalActive(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamBool* param = InParam->Cast<const FGsUIMsgParamBool>();
	if (nullptr == param) return;

	if(param->_data == true)
		GSInterServer()->ReqInvasionPortalInfo();
	else
	{
		auto mapId = GSWorldMap()->GetShowRegionMapId();

		if (false == GGameData()->IsInvadeWorld())
		{
			if (auto invadeMapdata = GSInterServer()->GetInvadeMapData(mapId).Pin())
			{
				for (auto& spot : invadeMapdata->_spawnPortals)
				{
					DespawnIconInvasionPortal(spot.id);
				}
			}
		}
	}
}

void UGsUIRegionIconRoot::OnUpdateInvasionPortalSpawnInfo(const IGsMessageParam* InParam)
{
	TArray<int64> portalSpots = {};
	_mapInvasionPortalIcons.GenerateKeyArray(portalSpots);

	for (auto spot : portalSpots)
	{
		DespawnIconInvasionPortal(spot);
	}

	MakeIconInvasionPortal();
}

void UGsUIRegionIconRoot::OnChangeRegionIconFiltering(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamRegionMapIconFiltering* param = InParam->Cast<const FGsUIMsgParamRegionMapIconFiltering>();
	if (nullptr == param)
	{
		return;
	}
		
	switch (param->_filteringType)
	{
	case EGsRegionMapIconFiltering::Party:
	{
		for (TPair<int64, UGsRegionMapIconBase*>& iter :_mapServerSpawnIcons)
		{
			// no local, party remote
			if (nullptr != iter.Value &&
				_localGameId != iter.Key)
			{				
				iter.Value->SetIconVisibility(param->_isVisible);
			}
		}

		for (TPair<int64, UGsRegionMapIconBase*>& iter : _mapPartyIcons)
		{
			if (nullptr != iter.Value )
			{
				iter.Value->SetIconVisibility(param->_isVisible);
			}
		}

		break;
	}
	case EGsRegionMapIconFiltering::TeleportSpot:
	{
		for (TPair<int64, UGsRegionMapIconSpot*>& iter : _mapTeleportIcons)
		{
			if (nullptr != iter.Value)
			{
				iter.Value->SetIconVisibility(param->_isVisible);
			}
		}
		break;
	}
	case EGsRegionMapIconFiltering::Bookmark:
	{
		for (TPair<uint64, UGsRegionMapIconSelect*>& iter : _mapBookmarkIcons)
		{
			if (nullptr != iter.Value)
			{
				iter.Value->SetIconVisibility(param->_isVisible);
			}
		}
		break;
	}
	case EGsRegionMapIconFiltering::SpaceCrackPortal:
	{
		for (TPair<int64, UGsRegionMapIconBase*>& iter : _mapSpaceCrackPortalIcons)
		{
			if (nullptr != iter.Value)
			{
				iter.Value->SetIconVisibility(param->_isVisible);
			}
		}
		break;
	}
	case EGsRegionMapIconFiltering::FieldBoss:
	{
		for (TPair<CreatureId, UGsRegionMapIconFieldBoss*>& iter : _mapFieldBossIcons)
		{
			if (nullptr != iter.Value)
			{
				iter.Value->SetIconVisibility(param->_isVisible);
			}
		}
		break;
	}
	case EGsRegionMapIconFiltering::Sanctum:
	{
		for (TPair<int32, UGsRegionMapIconBase*>& iter : _mapSanctumIcons)
		{
			if (nullptr != iter.Value)
			{
				iter.Value->SetIconVisibility(param->_isVisible);
			}
		}
		break;
	}
	}
	
}

void UGsUIRegionIconRoot::MonsterIconVisibleToggle(bool In_isVisible)
{
	for (TPair<int64, UGsRegionMapIconSelect*>& iter : _mapMonsterDropIcons)
	{
		if (nullptr != iter.Value)
		{
			iter.Value->SetIconVisibility(In_isVisible);
		}
	}
	// 켜는거는 옵션값도 체크해야함
	// 몬스터 끄는게 나머지 켜는거라서 false일때 나머지 켠다
	bool otherIconVisible = !In_isVisible;

	bool partyVisible = otherIconVisible;
	if (partyVisible == true)
	{
		partyVisible = FGsWorldMapFunc::IsRegionMapIconFilteringVisible(EGsRegionMapIconFiltering::Party);
	}

	for (TPair<int64, UGsRegionMapIconBase*>& iter : _mapServerSpawnIcons)
	{
		// no local, party remote
		if (nullptr != iter.Value &&
			_localGameId != iter.Key)
		{		
			iter.Value->SetIconVisibility(partyVisible);
		}
	}

	for (TPair<int64, UGsRegionMapIconBase*>& iter : _mapPartyIcons)
	{
		if (nullptr != iter.Value)
		{
			iter.Value->SetIconVisibility(partyVisible);
		}
	}

	for (TPair<int64, UGsRegionMapIconSpot*>& iter : _mapTeleportIcons)
	{
		if (nullptr != iter.Value)
		{
			bool teleportSpotVisible = otherIconVisible;
			if (teleportSpotVisible == true)
			{
				teleportSpotVisible = FGsWorldMapFunc::IsRegionMapIconFilteringVisible(EGsRegionMapIconFiltering::TeleportSpot);
			}
			iter.Value->SetIconVisibility(teleportSpotVisible);
		}
	}

	for (TPair<uint64, UGsRegionMapIconSelect*>& iter : _mapBookmarkIcons)
	{
		if (nullptr != iter.Value)
		{
			bool bookmarkVisible = otherIconVisible;
			if (bookmarkVisible == true)
			{
				bookmarkVisible = FGsWorldMapFunc::IsRegionMapIconFilteringVisible(EGsRegionMapIconFiltering::Bookmark);
			}
			iter.Value->SetIconVisibility(bookmarkVisible);
		}
	}

	for (TPair<int64, UGsRegionMapIconBase*>& iter : _mapSpaceCrackPortalIcons)
	{
		if (nullptr != iter.Value)
		{
			bool spacecrackVisible = otherIconVisible;
			if (spacecrackVisible == true)
			{
				spacecrackVisible = FGsWorldMapFunc::IsRegionMapIconFilteringVisible(EGsRegionMapIconFiltering::SpaceCrackPortal);
			}
			iter.Value->SetIconVisibility(spacecrackVisible);
		}
	}

	for (TPair<CreatureId, UGsRegionMapIconFieldBoss*>& iter : _mapFieldBossIcons)
	{
		if (nullptr != iter.Value)
		{
			bool fieldbossVisible = otherIconVisible;
			if (fieldbossVisible == true)
			{
				fieldbossVisible = FGsWorldMapFunc::IsRegionMapIconFilteringVisible(EGsRegionMapIconFiltering::FieldBoss);
			}
			iter.Value->SetIconVisibility(fieldbossVisible);
		}
	}

	// 포탈은 옵션에 안들어감
	for (TPair<int64, UGsRegionMapIconSelect*>& iter : _mapPortalIcons)
	{
		if (nullptr != iter.Value)
		{			
			iter.Value->SetIconVisibility(otherIconVisible);
		}
	}

	for (TPair<int32, UGsRegionMapIconBase*>& iter : _mapSanctumIcons)
	{
		if (nullptr != iter.Value)
		{
			bool sanctumVisible = otherIconVisible;
			if (sanctumVisible == true)
			{
				sanctumVisible = FGsWorldMapFunc::IsRegionMapIconFilteringVisible(EGsRegionMapIconFiltering::Sanctum);
			}
			iter.Value->SetIconVisibility(sanctumVisible);
		}
	}

}

void UGsUIRegionIconRoot::OnClickFieldBossIcon(CreatureId In_creatureId)
{
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();

	const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::GetFieldBossData(showRegionMapId, In_creatureId);

	if (bossData == nullptr)
	{
		return;
	}

	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::BOSS))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BOSS);
	}

	FGsGameStateBoss::GetInstance()->SelectedFieldBoss(bossData->bossId);
}

// invasion NPC(Guardian, Monster, Wagon)
void UGsUIRegionIconRoot::MakeIconInvasionNPC()
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

		EGsRegionMapIconType iconType = FGsMapFunc::GetInvasionNPCRegionMapIconType(iter.Value->_NPCType);

		if (iconType == EGsRegionMapIconType::None)
		{
			continue;
		}


		SpawnIconInvasionNPC(iter.Value, iconType);
	}
}

// spawn invasion npc
void UGsUIRegionIconRoot::SpawnIconInvasionNPC(FGsRegionInvadeIconInfo* In_iconInfo, EGsRegionMapIconType In_iconType)
{
	if (In_iconInfo == nullptr)
	{
		return;
	}

	EGsRegionInvadeNPCType npcType = In_iconInfo->_NPCType;


	UGsRegionMapIconBase* findIcon = _mapInvadeNPCIcons.FindRef(npcType);
	if (findIcon != nullptr)
	{
		DepspawnIconInvasionNPC(npcType);
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(In_iconType);

	UGsRegionMapIconInvasionNPC* invasionNPCIcon = NewObject<UGsRegionMapIconInvasionNPC>();

	invasionNPCIcon->SetInvasionNPCInfo(In_iconInfo);
	invasionNPCIcon->SetTimmer(data->_updateTime);

	
	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType, true);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		invasionNPCIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}
	

	_mapInvadeNPCIcons.Add(npcType, invasionNPCIcon);

	_listRegionIcons.Add(invasionNPCIcon);

	invasionNPCIcon->UpdatePos();
}
// despawn invasion npc
void UGsUIRegionIconRoot::DepspawnIconInvasionNPC(EGsRegionInvadeNPCType In_NPCType)
{
	UGsRegionMapIconBase* findIcon = _mapInvadeNPCIcons.FindRef(In_NPCType);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapInvadeNPCIcons.Remove(In_NPCType);
}

void UGsUIRegionIconRoot::OnInvasionNPC(const IGsMessageParam* InParam)
{
	const FGsMapInvasionNPCParam* paramTarget = InParam->Cast<const FGsMapInvasionNPCParam>();

	if (paramTarget->_isAdd)
	{
		AddInvasionNPC(paramTarget->_arrayNPCType);
	}
	else
	{
		DelInvasionNPC(paramTarget->_arrayNPCType);
	}
}

// add invasion npc 
void UGsUIRegionIconRoot::AddInvasionNPC(const TArray<EGsRegionInvadeNPCType> In_arrayType)
{
	for (EGsRegionInvadeNPCType iterType : In_arrayType)
	{
		FGsRegionInvadeIconInfo* findInfo = GSWorldMap()->GetRegionInvadeInfoByNPCType(iterType);
		if (findInfo == nullptr)
		{
			continue;
		}

		EGsRegionMapIconType iconType = FGsMapFunc::GetInvasionNPCRegionMapIconType(findInfo->_NPCType);

		if (iconType == EGsRegionMapIconType::None)
		{
			continue;
		}

		SpawnIconInvasionNPC(findInfo, iconType);
	}
}
// delete invasion npc
void UGsUIRegionIconRoot::DelInvasionNPC(const TArray<EGsRegionInvadeNPCType> In_arrayType)
{
	for (EGsRegionInvadeNPCType iterType : In_arrayType)
	{
		DepspawnIconInvasionNPC(iterType);
	}
}

void UGsUIRegionIconRoot::OnMonsterDropSpawn(const IGsMessageParam* InParam)
{
	const FGsMapMonsterDropIconParam* paramTarget = InParam->Cast<const FGsMapMonsterDropIconParam>();

	AddMonsterDropIcon(paramTarget->_mapId, paramTarget->_npcId, paramTarget->_spawnGroupIdArray, paramTarget->_posArray);
}

void UGsUIRegionIconRoot::AddMonsterDropIcon(MapId In_mapId, CreatureId In_npcId, const TArray<SpawnGroupId>& In_SpawnGroupIdArray, const TArray<FVector>& In_posArray)
{
	// 기존 몬스터 아이콘 삭제 처리
	_monsterIconCount = FMath::Max(_monsterIconCount, In_posArray.Num());
	for (int32 i = 0; i < _monsterIconCount; ++i)
	{
		UGsRegionMapIconBase* findIcon = _mapMonsterDropIcons.FindRef(i);
		if (findIcon != nullptr)
		{
			DespawnIconMonsterDrop(i);
		}
	}

	for (int i = 0; i < In_posArray.Num(); ++i)
	{
		if (0 == i)
		{
			SpawnIconMonsterDrop(i, In_mapId, In_npcId, In_SpawnGroupIdArray[i], In_posArray[i], EGsRegionMapIconType::MonsterDrop1, i);
		}
		else if (1 == i)
		{
			SpawnIconMonsterDrop(i, In_mapId, In_npcId, In_SpawnGroupIdArray[i], In_posArray[i], EGsRegionMapIconType::MonsterDrop2, i);
		}
		else
		{
			SpawnIconMonsterDrop(i, In_mapId, In_npcId, In_SpawnGroupIdArray[i], In_posArray[i], EGsRegionMapIconType::MonsterDrop3, i);
		}
	}
}

void UGsUIRegionIconRoot::SpawnIconMonsterDrop(int64 InSpotId, int32 InMapId, CreatureId InNpcId, int32 InSpawnGroupId, FVector InPos, EGsRegionMapIconType InIconType, int InSelectIndex)
{
	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(InIconType);

	UGsRegionMapIconSelectMonster* fixedIcon = NewObject<UGsRegionMapIconSelectMonster>();
	fixedIcon->SetMapId(InMapId);
	fixedIcon->SetNpcId(InNpcId);
	fixedIcon->SetSpawnGroupId(InSpawnGroupId);
	fixedIcon->SetActorPos(FVector2D(InPos));
	UGsUIIconBase* makeIcon = CreateMapIcon(InIconType);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		fixedIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}
	fixedIcon->SetSelect(false);
	//fixedIcon->SetSpotName(FText::GetEmpty());
	fixedIcon->SetSelectIndex(InSelectIndex);


	UGsUIIconRegionSelectButton* iconSelectButton = Cast<UGsUIIconRegionSelectButton>(makeIcon);
	if (iconSelectButton != nullptr)
	{
		iconSelectButton->SetClickCallback([this, fixedIcon, InPos]
			{
				OnClickMonsterDropIcon(fixedIcon, InPos);
			});
	}

	_mapMonsterDropIcons.Add(InSpotId, fixedIcon);

	_listRegionIcons.Add(fixedIcon);

	fixedIcon->UpdatePos();
}

void UGsUIRegionIconRoot::DespawnIconMonsterDrop(int64 InSpotId)
{
	UGsRegionMapIconBase* findIcon = _mapMonsterDropIcons.FindRef(InSpotId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapMonsterDropIcons.Remove(InSpotId);
}

// 몬스터 드롭위치에 아이콘 메뉴 열기
void UGsUIRegionIconRoot::ShowMonsterDropIconMenu(UGsRegionMapIconSelectMonster* In_iconObj, FVector In_pos)
{
	if (In_iconObj == nullptr)
	{
		return;
	}
	// icon pos
	UGsUIIconBase* icon = In_iconObj->GetIcon();
	if (icon == nullptr)
	{
		return;
	}

	FVector2D pos = icon->RenderTransform.Translation;

	UGsUIRegionIconMenu* findMenu = GetNowIconMenu(pos);
	if (findMenu != nullptr)
	{
		findMenu->SetRenderTranslation(pos);

		// 드랍 몬스터 데이터 세팅
		findMenu->SetMonsterDropData(
			In_iconObj->GetMapId(), In_iconObj->GetNpcId(), In_iconObj->GetSpawnGroupId(), In_pos);

		// menu visible 
		findMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}


	// hide touch menu
	HideTouchMenu();
}

// 성소 아이콘 만들기
void UGsUIRegionIconRoot::MakeIconSanctum()
{

	FGsWorldMapManager* worldmapManager = GSWorldMap();
	if (worldmapManager == nullptr)
	{
		return;
	}

	FGsRegionInfo* regionInfo =
		worldmapManager->GetCurrentRegionInfo();

	if (regionInfo == nullptr)
	{
		return;
	}

	// 침공전 넘어가서도 포털만 표시 요청
	// https://jira.com2us.com/jira/browse/CHR-23653
	FGsGameDataManager* dataManager = GGameData();
	if (dataManager == nullptr)
	{
		return;
	}
	bool isInvadeWorld = dataManager->IsInvadeWorld();

	// 성소 아이콘 만들기
	if (regionInfo->_arrayRegionMapSanctumInfo.Num() != 0 && isInvadeWorld == false)
	{
		for (auto iterSanctum : regionInfo->_arrayRegionMapSanctumInfo)
		{
			if (iterSanctum == nullptr)
			{
				continue;
			}

			TWeakPtr<FGsSanctumData> sanctumDataPtr = GSSanctum()->GetSanctumData(iterSanctum->_sanctumAreaId);
			if (false == sanctumDataPtr.IsValid())
			{
				continue;
			}

			const FGsSanctumData* sanctumData = sanctumDataPtr.Pin().Get();
			if (nullptr == sanctumData)
			{
				continue;
			}

			SpawnIconSanctum(sanctumData, iterSanctum);
		}
	}
}

// 성소 아이콘 만들기
void UGsUIRegionIconRoot::SpawnIconSanctum(const FGsSanctumData* In_sanctumData, FGsRegionMapSanctumInfo* In_regionSanctumInfo)
{
	if (In_sanctumData == nullptr ||
		In_regionSanctumInfo == nullptr)
	{
		return;
	}

	SanctumAreaId sanctumId = In_sanctumData->GetSanctumId();

	UGsRegionMapIconBase* findIcon = _mapSanctumIcons.FindRef(sanctumId);
	if (findIcon != nullptr)
	{
		DespawnIconSanctum(sanctumId);
	}

	const FGsSchemaRegionMapIconData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapIconData, FGsSchemaRegionMapIconData>(EGsRegionMapIconType::Sanctum);

	UGsRegionMapIconSelectSanctum* fixedIcon = NewObject<UGsRegionMapIconSelectSanctum>();
	FVector pos = In_regionSanctumInfo->_sanctumPos;

	fixedIcon->SetActorPos(FVector2D(pos));
	UGsUIIconBase* makeIcon = CreateMapIcon(EGsRegionMapIconType::Sanctum, true);

	if (makeIcon == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* iconSlot =
		_iconRoot->AddChildToCanvas(makeIcon);
	if (iconSlot != nullptr)
	{
		fixedIcon->SetIcon(makeIcon, iconSlot, data->_depth);
	}
	fixedIcon->SetSanctumInfo(In_sanctumData, In_regionSanctumInfo->_sanctumName);
	// 처음엔 선택 안되게 처리
	fixedIcon->SetSelect(false);
	UGsUIIconRegionSelectButtonSanctum* iconSanctum = Cast<UGsUIIconRegionSelectButtonSanctum>(makeIcon);
	if (iconSanctum != nullptr)
	{
		iconSanctum->SetClickCallback([this, fixedIcon]
		{
			OnClickSanctum(fixedIcon);
		});
	}


	_mapSanctumIcons.Add(sanctumId, fixedIcon);

	_listRegionIcons.Add(fixedIcon);

	fixedIcon->UpdatePos();
}

// 성소 아이콘 삭제
void UGsUIRegionIconRoot::DespawnIconSanctum(int32 In_sanctumAreaId)
{
	UGsRegionMapIconBase* findIcon = _mapSanctumIcons.FindRef(In_sanctumAreaId);
	if (findIcon == nullptr)
	{
		return;
	}

	FGsIconFactory::ReleaseIcon(findIcon->GetIcon());

	findIcon->Finalize();
	_listRegionIcons.RemoveSwap(findIcon);

	findIcon = nullptr;

	_mapSanctumIcons.Remove(In_sanctumAreaId);
}

// 성소 클릭
void UGsUIRegionIconRoot::OnClickSanctum(UGsRegionMapIconSelectSanctum* In_clickSanctumObj)
{
	// 아이콘 선택 표시 해제
	UnselectAllPortal();
	UnselectAllBookmark();
	UnselectAllSanctum();


	if (_callbackClickSanctum != nullptr)
	{
		_callbackClickSanctum(In_clickSanctumObj);
	}
}

// 아이콘 메뉴 show: 성소 아이콘 클릭으로
void UGsUIRegionIconRoot::ShowSanctumIconMenu(UGsRegionMapIconSelectSanctum* In_clickedIconObj)
{
	if (In_clickedIconObj == nullptr)
	{
		return;
	}

	// icon pos
	UGsUIIconBase* icon = In_clickedIconObj->GetIcon();
	if (icon == nullptr)
	{
		return;
	}
	// 클릭한 아이콘 위에 메뉴 띄우기
	FVector2D pos = icon->RenderTransform.Translation;

	UGsUIRegionIconMenu* findMenu = GetNowIconMenu(pos);
	if (findMenu != nullptr)
	{
		findMenu->SetRenderTranslation(pos);

		// select index 
		// 데이터 세팅
		SanctumAreaId selectIndex = In_clickedIconObj->GetSanctumId();
		findMenu->SetSanctumData(selectIndex);
		// menu visible 
		findMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}


	// hide touch menu
	// 땅찍고 이동 끄기
	HideTouchMenu();
}
// 성소 id로 아이콘 메뉴 show
void UGsUIRegionIconRoot::ShowSanctumIconMenuBySanctumId(SanctumAreaId In_id)
{
	UGsRegionMapIconBase* findIcon = _mapSanctumIcons.FindRef(In_id);
	if (findIcon == nullptr)
	{
		return;
	}

	UGsRegionMapIconSelectSanctum* sanctumIcon = Cast<UGsRegionMapIconSelectSanctum>(findIcon);
	if (sanctumIcon == nullptr)
	{
		return;
	}
	// 선택 표시 처리
	sanctumIcon->SetSelect(true);

	ShowSanctumIconMenu(sanctumIcon);
}
// 성소 정보 갱신됨
void UGsUIRegionIconRoot::OnUpdatedSanctumInfoUpdated(const IGsMessageParam*)
{
	// 아이콘 다시 만듬
	MakeIconSanctum();
}

// 현재 아이콘 위치로 아이콘 메뉴(이동, 워프) 가져오기(위/아래버전 선택)
UGsUIRegionIconMenu* UGsUIRegionIconRoot::GetNowIconMenu(FVector2D In_iconPos)
{
	// 0보다 크면 아래쪽 위치아이콘: 메뉴가 위쪽표시
	// 0보다 작으면 위쪽 위치아이콘: 메뉴가 아래쪽표시
	bool isUpperMenu =  (In_iconPos.Y > 0.f)? true: false;

	UGsUIRegionIconMenu* findMenu = _iconMenu;
	// 위쪽이면 아래쪽은 hide
	if (isUpperMenu == true)
	{
		_iconMenuDown->SetVisibility(ESlateVisibility::Hidden);
		findMenu = _iconMenu;
	}
	else
	{
		_iconMenu->SetVisibility(ESlateVisibility::Hidden);
		findMenu = _iconMenuDown;
	}
	return findMenu;
}

// 현재 아이콘 위치로 터치 메뉴(자동 이동) 가져오기(위/아래버전 선택)
UGsUIRegionTouchMenu* UGsUIRegionIconRoot::GetNowTouchMenu(FVector2D In_iconPos)
{

	// 0보다 크면 아래쪽 위치아이콘: 메뉴가 위쪽표시
	// 0보다 작으면 위쪽 위치아이콘: 메뉴가 아래쪽표시
	bool isUpperMenu = (In_iconPos.Y > 0.f) ? true : false;

	UGsUIRegionTouchMenu* findMenu = _touchMenu;
	// 위쪽이면 아래쪽은 hide
	if (isUpperMenu == true)
	{
		_touchMenuDown->SetVisibility(ESlateVisibility::Hidden);
		findMenu = _touchMenu;
	}
	else
	{
		_touchMenu->SetVisibility(ESlateVisibility::Hidden);
		findMenu = _touchMenuDown;
	}
	return findMenu;
}