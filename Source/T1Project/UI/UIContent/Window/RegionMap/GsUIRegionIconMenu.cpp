#include "GsUIRegionIconMenu.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Map/WorldMap/GsRegionInfo.h"

#include "Dungeon/GsDungeonData.h"

#include "UTIL/GsTableUtil.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonGroupData.h"

#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIRegionIconMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIRegionIconMenu::OnClickAutoMove);
	_btnTeleport->OnClicked.AddDynamic(this, &UGsUIRegionIconMenu::OnClickTeleport);
}

void UGsUIRegionIconMenu::NativeConstruct()
{
	Super::NativeConstruct();


}

void UGsUIRegionIconMenu::NativeDestruct()
{
	
	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	Super::NativeDestruct();
}


void UGsUIRegionIconMenu::OnClickAutoMove()
{
	if(_iconMenuType == EGsIconMenuType::NormalPortal)
	{ 
		FGsWorldMapFunc::ClickAutoMoveRegionMap(true, _listSelectIndex);
	}
	// 성소로 이동
	else if (_iconMenuType == EGsIconMenuType::Sanctum)
	{
		FGsRegionMapSanctumInfo* sanctumInfo =
			GSWorldMap()->GetCurrentRegionSanctumInfoBySanctumId(_sanctumId);
		if (sanctumInfo == nullptr)
		{
			return;
		}
		FGsWorldMapFunc::ClickAutoMoveRegionMapPortalPos(sanctumInfo->_sanctumPos, 
			sanctumInfo->_sanctumAcceptanceRange);
	}
	// 침공 포탈 
	else if(_iconMenuType == EGsIconMenuType::InvasionPortal)
	{
		FGsWorldMapFunc::ClickAutoMoveRegionMapPortalPos(_movePos);
	}	
	// 몬스터 드랍 위치 이동
	else if (_iconMenuType == EGsIconMenuType::MonsterPos)
	{
		FGsWorldMapFunc::ClickAutoMoveRegionMapPortalPos(_movePos, _monsterDropAcceptanceRange);
	}
}

void UGsUIRegionIconMenu::OnClickTeleport()
{
	if(_iconMenuType == EGsIconMenuType::NormalPortal)
	{ 
		FGsPortalInfo* portalInfo =
			GSWorldMap()->GetCurrentRegionPortalInfoByIndex(_listSelectIndex);

		if (portalInfo == nullptr)
		{
			return;
		}

		FGsWorldMapFunc::ClickTeleportRegionMap(true, _listSelectIndex, portalInfo->_portalName.ToString());
	}
	else if (_iconMenuType == EGsIconMenuType::InvasionPortal)
	{
		FGsWorldMapFunc::ClickTeleportRegionMapInvasionPortal(_invasionSpotId, _invasionPortalText);
	}
	else if (_iconMenuType == EGsIconMenuType::MonsterPos)
	{
		FGsWorldMapFunc::ClickTeleportRegionMapMonster(
			_monsterDropMapId, _monsterDropCreatureId, _monsterDropSpawnGroupId);
	}
}

void UGsUIRegionIconMenu::SetNormalPortalData(int In_index)
{
	_listSelectIndex = In_index;

	_iconMenuType = EGsIconMenuType::NormalPortal;

	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(_aniMenu);

	InvalidateTeleportState();
}

// 침공 포탈 데이터 세팅
void UGsUIRegionIconMenu::SetInvasionPortalData(int64 In_portalSpotId, FVector In_portalPos,
	FString In_invasionText)
{
	_invasionSpotId = In_portalSpotId;
	_movePos = In_portalPos;
	_invasionPortalText = In_invasionText;

	_iconMenuType = EGsIconMenuType::InvasionPortal;

	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(_aniMenu);

	InvalidateTeleportState();
}

// 몬스터 드랍 데이터 세팅
void UGsUIRegionIconMenu::SetMonsterDropData(MapId In_mapId, CreatureId In_npcId, 
	SpawnGroupId In_spawnGroupId, FVector In_pos)
{
	_movePos = In_pos;

	_monsterDropMapId = In_mapId;
	_monsterDropCreatureId = In_npcId;
	_monsterDropSpawnGroupId = In_spawnGroupId;

	_iconMenuType = EGsIconMenuType::MonsterPos;

	if (const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(In_npcId))
	{
		if (const FGsSchemaCreatureCommonInfo* creatureCommonInfo = npcData->commonInfoId.GetRow())
		{
			float addVal  = GData()->GetGlobalData()->_addMonsterDropMoveAcceptanceRange;
			_monsterDropAcceptanceRange = creatureCommonInfo->cylinderRadius + addVal;
		}
	}

	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(_aniMenu);

	InvalidateTeleportState();
}

// 텔레포트 버튼 상태 갱신(던전은 사용x, 아니면 강제로 딤드 만들지)
void UGsUIRegionIconMenu::InvalidateTeleportState(bool In_isForcedDimmed)
{
	bool isDungeon = FGsWorldMapFunc::IsRegionDungeon();

	// 일반던전인경우는 워프 사용하게 변경
	// https://jira.com2us.com/jira/browse/CHR-25993
	// 1. 일반던전인경우: 해금되지 않았을때, 던전 이용시간 남지 않았을때 -> 불가
	// 2. 나머지 던전은 불가
	// 3. 강제로 세팅

	bool isTeleportPossible = false;

	if (isDungeon == true)
	{
		int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
		const FGsSchemaDungeonGroupData* findDungeonGroupTbl = GSWorldMap()->FindDungeonGroupTblByMapId(showRegionMapId);
		if (findDungeonGroupTbl != nullptr)
		{
			if (findDungeonGroupTbl->dungeonCategory == DungeonCategory::NORMAL)
			{
				// 1. 던전 남은시간, 언락 컨텐츠(던전 그룹의 언락) 체크
				bool isGroupWarpPossible = GSDungeon()->IsPossibleWarpByMapId(showRegionMapId, false);
				if (isGroupWarpPossible == true)
				{
					// 2. 각층의 언락 상태 체크(레벨 제한)
					const FGsDungeonData* findDungeonData = GSDungeon()->FindDungeonDataByMapId(showRegionMapId);
					if (findDungeonData != nullptr &&  true == findDungeonData->IsUnLockState() )
					{
						isTeleportPossible = true;
					}
					else
					{
						isTeleportPossible = false;
					}
				}
				else
				{
					isTeleportPossible = false;
				}
			}
			else
			{
				isTeleportPossible = false;
			}
		}
	}
	else if (In_isForcedDimmed == true)
	{
		isTeleportPossible = false;
	}
	else
	{
		isTeleportPossible = true;
	}


	_switcherTeleport->SetActiveWidgetIndex(
		(isTeleportPossible == true)? TELEPORT_POSSIBLE: TELEPORT_IMPOSSIBLE);
}

// 성소 데이터 세팅
void UGsUIRegionIconMenu::SetSanctumData(SanctumAreaId In_id)
{
	_sanctumId = In_id;

	_iconMenuType = EGsIconMenuType::Sanctum;

	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(_aniMenu);

	InvalidateTeleportState(true);
}