#include "GsWorldMapFunc.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsObserverManager.h"

#include "Message/GsMessageContents.h"

#include "Option/GsServerOption.h"

#include "Map/GsMapFunc.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Map/Bookmark/GsBookmarkFunc.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Party/GsPartyMemberInfo.h"

#include "Currency/GsCurrencyHelper.h"

#include "DataSchema/Map/GsSchemaMapData.h"

#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Bookmark/GsBookmarkHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "Currency/GsCostPackage.h"

#include "GameObject/Boss/GsBossHandler.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


// 파티 아이콘 타입 얻어오기
bool FGsWorldMapFunc::GetPartyIconType(int64 In_gameId, EGsRegionMapIconType& Out_iconType)
{
	FGsPartyMemberInfo* memberInfo = GSParty()->FindPartyMemberInfoByGameId(In_gameId);
	if (nullptr == memberInfo)
	{
		Out_iconType = EGsRegionMapIconType::None;
		return false;
	}
	int slotId = memberInfo->GetPartySlotId();
	int memberChannelId = memberInfo->GetChannelId();
	int localChannelId = GGameData()->GetCurrentChannel();
	bool isSameChannelWithLocal = (memberChannelId == localChannelId) ? true : false;
	switch (slotId)
	{
	case 0:
		Out_iconType = (isSameChannelWithLocal == true) ? EGsRegionMapIconType::Party1 : EGsRegionMapIconType::Party1_OtherChannel;
		break;
	case 1:
		Out_iconType = (isSameChannelWithLocal == true) ? EGsRegionMapIconType::Party2 : EGsRegionMapIconType::Party2_OtherChannel;
		break;
	case 2:
		Out_iconType = (isSameChannelWithLocal == true) ? EGsRegionMapIconType::Party3 : EGsRegionMapIconType::Party3_OtherChannel;
		break;
	case 3:
		Out_iconType = (isSameChannelWithLocal == true) ? EGsRegionMapIconType::Party4 : EGsRegionMapIconType::Party4_OtherChannel;
		break;
	}

	return true;
}

// 지역 던전맵인가(둘중 하나라도 던전이면 던전)
// 1. 현재맵 -> 현재맵은 뺀다(던전에서 다른맵으로 나갈때 봐야 하므로)
// 2. 보고 있는 지역맵
bool FGsWorldMapFunc::IsRegionDungeon()
{
	EGsWorldMapGroupType groupType = GSWorldMap()->GetShowRegionMapGroupType();
	bool isShowRegionMapDungeon = (groupType == EGsWorldMapGroupType::Dungeon)? true : false;
	return isShowRegionMapDungeon;
}

// get visible by region icon type(check server option)
bool FGsWorldMapFunc::IsRegionMapIconTypeVisible(EGsRegionMapIconType In_iconType)
{
	WorldMapFilterInfo& optionData = GServerOption()->GetWorldMapFilter();
	bool isChecked = true;
	switch (In_iconType)
	{
	case EGsRegionMapIconType::Party1:
	case EGsRegionMapIconType::Party2:
	case EGsRegionMapIconType::Party3:
	case EGsRegionMapIconType::Party4:
	case EGsRegionMapIconType::Party1_OtherChannel:
	case EGsRegionMapIconType::Party2_OtherChannel:
	case EGsRegionMapIconType::Party3_OtherChannel:
	case EGsRegionMapIconType::Party4_OtherChannel:
		isChecked = optionData.mParty;
		break;
	case EGsRegionMapIconType::TeleportSpot:
		isChecked = optionData.mTeleportSpot;
		break;
	case EGsRegionMapIconType::Bookmark:
		isChecked = optionData.mBookMark;
		break;
	case EGsRegionMapIconType::SpaceCrackPortal:
		isChecked = optionData.mSpaceCrackPortal;
		break;
	case EGsRegionMapIconType::FieldBoss:
		isChecked = optionData.mFieldBoss;
		break;
	case EGsRegionMapIconType::Sanctum:
		isChecked = optionData.mSanctum;
		break;
	}
	return isChecked;
}
// 필터링 값 체크
bool FGsWorldMapFunc::IsRegionMapIconFilteringVisible(EGsRegionMapIconFiltering In_iconFiltering)
{
	WorldMapFilterInfo& optionData = GServerOption()->GetWorldMapFilter();
	bool isChecked = true;
	switch (In_iconFiltering)
	{
	case EGsRegionMapIconFiltering::Party:
		isChecked = optionData.mParty;
		break;
	case EGsRegionMapIconFiltering::TeleportSpot:
		isChecked = optionData.mTeleportSpot;
		break;
	case EGsRegionMapIconFiltering::Bookmark:
		isChecked = optionData.mBookMark;
		break;
	case EGsRegionMapIconFiltering::SpaceCrackPortal:
		isChecked = optionData.mSpaceCrackPortal;
		break;
	case EGsRegionMapIconFiltering::FieldBoss:
		isChecked = optionData.mFieldBoss;
		break;
	case EGsRegionMapIconFiltering::Sanctum:
		isChecked = optionData.mSanctum;
		break;
	}
	return isChecked;
}

// 몬스터 드랍 켜져있는지(체크할 타입들이 있음)
bool FGsWorldMapFunc::IsMonsterDropOn(EGsRegionMapIconType In_iconType)
{
	// 몬스터 드랍 필터 체크 해야할 타입인지
	bool isMonsterDropCheckedType = false;

	// 현재 몬스터 드랍 필터 중인지
	bool isMonsterDropIconFiltered = GSWorldMap()->GetIsMonsterDropIconFiltered();
	switch (In_iconType)
	{
	case EGsRegionMapIconType::Party1:
	case EGsRegionMapIconType::Party2:
	case EGsRegionMapIconType::Party3:
	case EGsRegionMapIconType::Party4:
	case EGsRegionMapIconType::Party1_OtherChannel:
	case EGsRegionMapIconType::Party2_OtherChannel:
	case EGsRegionMapIconType::Party3_OtherChannel:
	case EGsRegionMapIconType::Party4_OtherChannel:
		isMonsterDropCheckedType = true;
		break;
	case EGsRegionMapIconType::TeleportSpot:
		isMonsterDropCheckedType = true;
		break;
	case EGsRegionMapIconType::Bookmark:
		isMonsterDropCheckedType = true;
		break;
	case EGsRegionMapIconType::SpaceCrackPortal:
		isMonsterDropCheckedType = true;
		break;
	case EGsRegionMapIconType::FieldBoss:
		isMonsterDropCheckedType = true;
		break;
	case EGsRegionMapIconType::Sanctum:
		isMonsterDropCheckedType = true;
		break;
	case EGsRegionMapIconType::Portal:
		isMonsterDropCheckedType = true;
		break;
	}

	bool isMonsterDropOn = false;
	if (isMonsterDropCheckedType == true &&
		isMonsterDropIconFiltered == true)
	{
		isMonsterDropOn = true;
	}

	return isMonsterDropOn;
}

void FGsWorldMapFunc::ClickAutoMoveRegionMap(bool In_isPortal, int In_realSelUseIndex)
{
	if (true == FGsMapFunc::IsLocalMoveImpossible())
	{
		return;
	}

	// 대화연출 응답 대기중이면 return
	if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
	{
		return;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}


	if (In_isPortal == false)
	{
		// auto move to book mark pos
		FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
		if (nullptr == hud)
		{
			return;
		}

		FGsBookmarkHandler* bookmarkHandler = hud->GetBookmarkHandler();

		if (bookmarkHandler == nullptr)
		{
			return;
		}
		bookmarkHandler->MoveToBookmarkRegionMap(In_realSelUseIndex);
	}
	else
	{
		// auto move to portal pos
		if (FGsGameStateRegionMap* regionMap =
			contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
		{
			regionMap->MoveToPortal(In_realSelUseIndex);
		}

	}


	// main hud로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	// region map window 닫기
	// 특정 ui 만 닫으면 그전에 있던 ui가 뜨므로 다닫기로 한다
	// ex)지역->대륙->지역에서 이동시
	GUI()->CloseAllStack();
}

void FGsWorldMapFunc::ClickTeleportRegionMap(bool In_isPortal, int In_realSelUseIndex, FString In_targetName)
{
	// local이  상태이상이면 못보냄
	if (true == FGsMapFunc::IsLocalWarpImpossible())
	{
		return;
	}

	TSharedRef<FGsCostPackage> CostPackage(MakeShared<FGsCostPackage>());
	int useCost = 0;
	CurrencyType currencyType = CurrencyType::GOLD;
	EGsWorldmapPopupWarpType popupWarpType = EGsWorldmapPopupWarpType::Bookmark;
	if (false == In_isPortal)
	{
		CostPackage->AddCost(ItemCategorySub::WARP_TO_BOOKMARK_SCROLL, 1);
		popupWarpType = EGsWorldmapPopupWarpType::Bookmark;
	}
	else
	{
		int currentMapId = GSWorldMap()->GetShowRegionMapId();
		const FGsSchemaMapData* mapTbl = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(currentMapId);
		const FGsSchemaMapBalance* mapBalanceData = mapTbl->balanceId.GetRow();
		if (mapBalanceData != nullptr)
		{
			currencyType =
				mapBalanceData->cost.costCurrencyType;
			useCost = mapBalanceData->cost.costCurrencyAmount;
		}
		CostPackage->AddCost(currencyType, useCost);

		popupWarpType = EGsWorldmapPopupWarpType::Portal;
	}

	GSWorldMap()->ReserveWarpRegionItem(
		popupWarpType,
		In_targetName,
		MoveTemp(CostPackage),
		In_realSelUseIndex);
}

void FGsWorldMapFunc::ClickTeleportRegionMapMonster(MapId In_mapId, CreatureId In_npcId, SpawnGroupId In_spawnGroupId)
{
	// local이  상태이상이면 못보냄
	if (true == FGsMapFunc::IsLocalWarpImpossible())
	{
		return;
	}

	EGsWorldmapPopupWarpType popupWarpType;
	popupWarpType = EGsWorldmapPopupWarpType::MonsterDrop;


	MapId currentMapId = GSWorldMap()->GetShowRegionMapId();
	const FGsSchemaMapData* mapTbl = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(currentMapId);
	FString moveTargetText((mapTbl->MapStringId).ToString());

	
	TSharedRef<FGsCostPackage> CostPackage(MakeShared<FGsCostPackage>());
	int useCost = 0;
	CurrencyType currencyType = CurrencyType::NONE;

	const FGsSchemaMapBalance* mapBalanceData = mapTbl->balanceId.GetRow();
	if (mapBalanceData != nullptr)
	{
		// [B1] | ejrrb10 | 아이템 획득처 -> 몬스터 별 스폰 위치 이동시, 보스의 경우 재화를 다이아로 설정 (C2URWQ-4235)
		bool isBoss = FGsBossHandler::IsFieldBossSpawnGroup(In_mapId, In_spawnGroupId);
	
		if (isBoss)
		{
			currencyType = mapBalanceData->bossWarpCost.costCurrencyType;
			useCost = mapBalanceData->bossWarpCost.costCurrencyAmount;
		}
		else
		{
			currencyType = mapBalanceData->cost.costCurrencyType;
			useCost = mapBalanceData->cost.costCurrencyAmount;
		}
	}
	CostPackage->AddCost(currencyType, useCost);
	
	
	GSWorldMap()->ReserveWarpRegionMonster(
		popupWarpType,
		moveTargetText,
		MoveTemp(CostPackage),
		In_mapId,
		In_npcId,
		In_spawnGroupId);
}

// 지역맵 열기 가능한지
// 1. 언락 컨텐츠 체크
// 2. 불가능한 레벨인지 체크(MapType::INSTANCE_SINGLE || MapType::SINGLE_FIELD || MapType::GUILD_DUNGEON)
// 3. 관전모드일때 막기
bool FGsWorldMapFunc::IsRegionMapOpenPossible()
{
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::WORLDMAP))
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::WORLDMAP);
		return false;
	}

	// 개인 이벤트 채널이면 티커출력후 안넘어감
	// 싱글 필드면 안넘어감
	// 관전 모드면 안넘어감
	// or guild dungeon block
	if (GLevel()->IsMapType(MapType::INSTANCE_SINGLE) == true ||
		GLevel()->IsMapType(MapType::SINGLE_FIELD) == true ||
		GLevel()->IsMapType(MapType::GUILD_DUNGEON) == true ||
		GSObserver()->GetIsObserveStart() == true)
	{
		FText findText;
		FText::FindText(TEXT("WorldMapText"), TEXT("ErrorInvalidRegion"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	return true;
}

// 침공전 포탈: 자동 이동
// 몬스터 드랍 위치: 자동 이동
// 성소 위치 자동 이동
// 공용 함수명으로 변경
void FGsWorldMapFunc::ClickAutoMoveRegionMapPortalPos(FVector In_portalPos, float In_useAcceptanceRadius)
{
	if (true == FGsMapFunc::IsLocalMoveImpossible())
	{
		return;
	}

	// 대화연출 응답 대기중이면 return
	if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
	{
		return;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	// auto move to portal pos
	if (FGsGameStateRegionMap* regionMap =
		contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
	{
		regionMap->MoveToPortalPos(In_portalPos, In_useAcceptanceRadius);
	}


	// main hud로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	// region map window 닫기
	// 특정 ui 만 닫으면 그전에 있던 ui가 뜨므로 다닫기로 한다
	// ex)지역->대륙->지역에서 이동시
	GUI()->CloseAllStack();
}
// 침공전 포탈: 워프 
void FGsWorldMapFunc::ClickTeleportRegionMapInvasionPortal(int64 In_spotId, FString In_targetName)
{
	// local이  상태이상이면 못보냄
	if (true == FGsMapFunc::IsLocalWarpImpossible())
	{
		return;
	}

	TSharedRef<FGsCostPackage> CostPackage(MakeShared<FGsCostPackage>());
	int useCost = 0;
	CurrencyType currencyType = CurrencyType::GOLD;
	EGsWorldmapPopupWarpType popupWarpType = EGsWorldmapPopupWarpType::Bookmark;

	int currentMapId = GSWorldMap()->GetShowRegionMapId();
	const FGsSchemaMapData* mapTbl = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(currentMapId);
	const FGsSchemaMapBalance* mapBalanceData = mapTbl->balanceId.GetRow();
	if (mapBalanceData != nullptr)
	{
		currencyType =
			mapBalanceData->cost.costCurrencyType;
		useCost = mapBalanceData->cost.costCurrencyAmount;
	}
	CostPackage->AddCost(currencyType, useCost);

	popupWarpType = EGsWorldmapPopupWarpType::InvasionPortal;
	

	GSWorldMap()->ReserveWarpInvasionPortal(
		popupWarpType,
		In_targetName,
		MoveTemp(CostPackage),
		In_spotId);
}

// 위치 즐겨 찾기 자동 이동
// https://jira.com2us.com/jira/browse/CHR-23756
void FGsWorldMapFunc::ClickAutoMoveBookMarkPopup(int In_bookmarkIndex)
{
	if (true == FGsMapFunc::IsLocalMoveImpossible())
	{
		return;
	}

	// 대화연출 응답 대기중이면 return
	if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
	{
		return;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}
	
	// auto move to book mark pos
	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsBookmarkHandler* bookmarkHandler = hud->GetBookmarkHandler();

	if (bookmarkHandler == nullptr)
	{
		return;
	}
	bookmarkHandler->AutoMoveToBookmarkPopup(In_bookmarkIndex);
	
}