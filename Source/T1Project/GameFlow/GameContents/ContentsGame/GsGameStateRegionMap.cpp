#include "GsGameStateRegionMap.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "UI/UIContent/Window/GsUIWindowRegionMap.h"

#include "UI/UIContent/Popup/GsUIPopupMonsterInfo.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsRegionMonsterInfo.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Map/WorldMap/GsWorldMapFunc.h"

#include "Item/GsItem.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsItemUtil.h"
#include "UTIL/GsDir.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossRewardData.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnElement.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardBox.h"
#include "DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"
#include "DataSchema/MonsterDropItem/GsSchemaMonsterToMapData.h"
#include "DataSchema/Sanctum/GsSchemaSanctumData.h"

#include "Data/GsDataContainManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/GsAutoMoveInfo.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"

#include "GameObject/Boss/GsFieldBossHandler.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"

#include "Runtime/Public/GsTable.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Message/MessageParam/GsUIMessageParam.h"

FGsGameStateRegionMap::FGsGameStateRegionMap() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsRegionMap)
{	
}


FGsGameStateRegionMap::~FGsGameStateRegionMap()
{
}

void FGsGameStateRegionMap::Enter()
{
	Super::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowRegionMap"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowRegionMap>(widget);
		}
	}

	// hide navigation bar(22/03/10)
	// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=266919064
	CloseNavigationBar();

	if (FGsMessageHolder* msg = GMessage())
	{
		_uiDelegates.Emplace(msg->GetUI().AddRaw(MessageUI::CHANGE_REGION_MAP,
			this, &FGsGameStateRegionMap::OnChangeRegionMap));

		_uiDelegates.Emplace(msg->GetUI().AddRaw(MessageUI::CHANGE_REGION_MAP_SCENE,
			this, &FGsGameStateRegionMap::OnChangeRegionMapScene));

		_bossDelegates.Emplace(msg->GetBoss().AddRaw(MessageBoss::SHOW_BOSS_SPAWN_REGION,
			this, &FGsGameStateRegionMap::OnChangeRegionMap));		
	}

	//bind 
}
void FGsGameStateRegionMap::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_windowUI = nullptr;

	// 바인딩 된 메시지 해제
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_uiDelegates.Num() != 0)
		{
			for (auto iter : _uiDelegates)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegates.Empty();
		}

		if (_bossDelegates.Num() != 0)
		{
			for (auto iter : _bossDelegates)
			{
				msg->GetBoss().Remove(iter);
			}
			_bossDelegates.Empty();
		}
	}

	Super::Exit();
}

// MOVE_REGION_MAP_MYPOS
void FGsGameStateRegionMap::OnChangeRegionMap(const IGsMessageParam*)
{
	if (_windowUI.IsValid())
	{
		_windowUI->ChangeRegionMap();
	}
}

// 지역정보 만들기
FGsRegionInfo* FGsGameStateRegionMap::MakeRegionInfo(int In_mapId)
{
	// 포털 데이터 세팅
	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(In_mapId);
	if (mapData == nullptr)
	{
		return nullptr;
	}
	const FGsSchemaMapTerritoryData* data = mapData->territoryInfo.GetRow();

	if (data == nullptr)
	{
		return nullptr;
	}

	// 텔레포트가 없는 맵(아라크 던전, 환각의 사원) 도 있다고 함
	//if (data->teleportList.Num() == 0)
	//{
	//	return nullptr;
	//}

	// 포털은 1개 밖에 없다
	// 2개 이상이면 ui 구조 변경 되어야 함
	// 포털 0~4개 즉 동적으로 바뀜(2021/01/26)

	FGsClientSpawnManager* spawnManager = GSClientSpawn();
	if (spawnManager == nullptr)
	{
		return nullptr;
	}
	FGsRegionInfo* newInfo = new FGsRegionInfo();
	newInfo->_mapId = In_mapId;

	bool isThereATown = false;
	for (FGsSchemaMapSpotInfo mapSpotInfo: data->teleportList)
	{		
		FGsSchemaSpotInfo findSpotInfo;
		if (false == spawnManager->TryGetSpot(
			mapSpotInfo.mapId.GetRow()->mapId, mapSpotInfo.spotIndex, findSpotInfo))
		{
			continue;
		}

		FGsPortalInfo* newPortalInfo = new FGsPortalInfo();

		newPortalInfo->_portalName = findSpotInfo.name;
		newPortalInfo->_portalPos = findSpotInfo.pos;
		newPortalInfo->_portalSpotId = findSpotInfo.id;
		newPortalInfo->_isInTown = findSpotInfo.isInTown;

		newInfo->_arrayPortalInfo.Add(newPortalInfo);

		if (newPortalInfo->_isInTown == true)
		{
			isThereATown = true;
		}
		// 침공전 넘어갔을때는 마을 포탈 빼고 사용
		// https://jira.com2us.com/jira/browse/CHR-23653
		else
		{
			// 참조만 하고 실제 delete는 위에 _arrayPortalInfo서 하자
			newInfo->_arrayInvasionEnterPortalInfo.Add(newPortalInfo);
		}
	}

	newInfo->_isThereATown = isThereATown;

	// 텔레포트 정보 저장
	for (auto iter : data->warpList)
	{
		FGsSchemaSpotInfo teleSpotInfo;
		if (false == spawnManager->TryGetSpot(
			iter.sourceSpot.MapId.GetRow()->mapId, iter.sourceSpot.SpotIndex, teleSpotInfo))
		{
			continue;
		}
		FGsTeleportInfo* newTeleportInfo = new FGsTeleportInfo();
		newTeleportInfo->_teleportName = teleSpotInfo.name;
		newTeleportInfo->_teleportPos = teleSpotInfo.pos;
		newTeleportInfo->_teleportSpotId = teleSpotInfo.id;

		newInfo->_arrayTeleportInfo.Add(newTeleportInfo);
	}

	// 몬스터 정보 저장(몬스터 등급(보스), 이름순 정렬 때문에)
	const FGsSchemaWorldMapMonsterData* monsterData =
		UGsTableUtil::FindRowById<UGsTableWorldMapMonsterData, FGsSchemaWorldMapMonsterData>(In_mapId);

	bool isInvadePossibleMap = IsInvadePossibleMap(In_mapId);

	if (monsterData != nullptr)
	{
		for (const FGsSchemaWorldMapMonsterElementDataRow& iter : monsterData->monsters)
		{
			if (nullptr == iter.GetRow())
			{
				continue;
			}

			const FGsSchemaWorldMapMonsterElementData& WorldMapMonsterData = *iter.GetRow();
			const FGsSchemaNpcData* npcData = WorldMapMonsterData.id.GetRow();
			if (npcData == nullptr)
			{
				continue;
			}

			int monsterLevel = WorldMapMonsterData.level;

			FGsRegionMonsterInfo* newMonsterInfo = new FGsRegionMonsterInfo();
			newMonsterInfo->_npcData = npcData;
			newMonsterInfo->_fieldBossData = nullptr;
			newMonsterInfo->_isOffensive = WorldMapMonsterData.isOffensive;
			newMonsterInfo->level = monsterLevel;
			const FGsSchemaNpcBalance* npcBalanceTbl = npcData->balanceId.GetRow();
			if (npcBalanceTbl != nullptr)
			{
				newMonsterInfo->_npcBalance = npcBalanceTbl;
			}

			// 필드보스는 별도 보상 테이블에 접근한다
			if (npcData->npcFunctionType == NpcFunctionType::FIELD_BOSS)
			{
				// field boss only use normal type
				FGsRegionMonsterItemInfoSet* itemInfoSet = new FGsRegionMonsterItemInfoSet();
				newMonsterInfo->_mapItemInfoSet.Add(EGsRegionItemInfoSetType::Normal,itemInfoSet);

				const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::GetFieldBossData(In_mapId, npcData->id);
				if (bossData)
				{
					newMonsterInfo->_fieldBossData = bossData;

					for (const auto& rewardIter: bossData->bossRewardIdList)
					{
						const FGsSchemaFieldBossRewardData* fieldBossData = rewardIter.GetRow();
						if (nullptr == fieldBossData)
						{
							continue;
						}
						const FGsSchemaRewardData* rewardData = fieldBossData->rewardId.GetRow();
						if (nullptr == rewardData)
						{
							continue;
						}
						for (const auto& rewardBoxIter: rewardData->rewardBoxList)
						{
							const FGsSchemaRewardBoxData* boxData = rewardBoxIter.rewardBoxDataRow.GetRow();
							if (nullptr == boxData)
							{
								continue;
							}

							for (const auto& itemBagDataIter: boxData->itemBagList)
							{
								int itemId = itemBagDataIter.itemId;
								const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(itemId);

								if (commonData == nullptr)
								{
									continue;
								}

								// 통합
								// https://jira.com2us.com/jira/browse/CHR-23756
								if (itemInfoSet->_arrayItem.ContainsByPredicate([commonData](FGsRegionMonsterItemInfo* el)
									{
										return el->_itemTbl == commonData;
									}) == false)
								{
									FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(commonData);
									itemInfoSet->_arrayItem.Add(newData);
								}
								


							}
						}
					}
				}
			}			
			else
			{
				// normal map
				FGsRegionMonsterItemInfoSet* normalItemInfoSet = new FGsRegionMonsterItemInfoSet();
				newMonsterInfo->_mapItemInfoSet.Add(EGsRegionItemInfoSetType::Normal, normalItemInfoSet);

				// 획득 아이템 정보 저장(등급순 정렬 때문에)
				for (auto equipItemIter : WorldMapMonsterData.equipItemList)
				{

					// 월드맵 개선 통합으로 출력
					// https://jira.com2us.com/jira/browse/CHR-23756
					const FGsSchemaItemCommon* equipItemTbl = equipItemIter.GetRow();
					if (equipItemTbl == nullptr)
					{
						continue;
					}

					FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(equipItemTbl);
					normalItemInfoSet->_arrayItem.Add(newData);
				}

				for (auto etcItemIter : WorldMapMonsterData.etcItemList)
				{
					// 월드맵 개선 통합으로 출력
					// https://jira.com2us.com/jira/browse/CHR-23756
					const FGsSchemaItemCommon* etcItemTbl = etcItemIter.GetRow();
					if (etcItemTbl == nullptr)
					{
						continue;
					}

					FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(etcItemTbl);
					normalItemInfoSet->_arrayItem.Add(newData);
				}

				// check if invade possible map 
				if (isInvadePossibleMap == true)
				{
					FGsRegionMonsterItemInfoSet* attackItemInfoSet = new FGsRegionMonsterItemInfoSet();
					newMonsterInfo->_mapItemInfoSet.Add(EGsRegionItemInfoSetType::Invade_Attack, attackItemInfoSet);

					// add invade attack item
					for (auto invadeAttackEquipItemIter : WorldMapMonsterData.invasionAttackEquipItemList)
					{

						// 월드맵 개선 통합으로 출력
						// https://jira.com2us.com/jira/browse/CHR-23756
						const FGsSchemaItemCommon* equipItemTbl = invadeAttackEquipItemIter.GetRow();
						if (equipItemTbl == nullptr)
						{
							continue;
						}

						FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(equipItemTbl);
						attackItemInfoSet->_arrayItem.Add(newData);
					}
					for (auto invadeAttackEtcItemIter : WorldMapMonsterData.invasionAttackEtcItemList)
					{

						// 월드맵 개선 통합으로 출력
						// https://jira.com2us.com/jira/browse/CHR-23756
						const FGsSchemaItemCommon* etcItemTbl = invadeAttackEtcItemIter.GetRow();
						if (etcItemTbl == nullptr)
						{
							continue;
						}

						FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(etcItemTbl);
						attackItemInfoSet->_arrayItem.Add(newData);

					}

					// add normal item info
					for (auto equipItemIter : WorldMapMonsterData.equipItemList)
					{

						const FGsSchemaItemCommon* equipItemTbl = equipItemIter.GetRow();
						if (equipItemTbl == nullptr)
						{
							continue;
						}

						FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(equipItemTbl);
						attackItemInfoSet->_arrayItem.Add(newData);

					}

					for (auto etcItemIter : WorldMapMonsterData.etcItemList)
					{
						const FGsSchemaItemCommon* etcItemTbl = etcItemIter.GetRow();
						if (etcItemTbl == nullptr)
						{
							continue;
						}

						FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(etcItemTbl);
						attackItemInfoSet->_arrayItem.Add(newData);
					}

					FGsRegionMonsterItemInfoSet* defenseItemInfoSet = new FGsRegionMonsterItemInfoSet();
					newMonsterInfo->_mapItemInfoSet.Add(EGsRegionItemInfoSetType::Invade_Defense, defenseItemInfoSet);


					// add invade defense item
					for (auto invadeDefenseEquipItemIter : WorldMapMonsterData.invasionDefenceEquipItemList)
					{

						const FGsSchemaItemCommon* equipItemTbl = invadeDefenseEquipItemIter.GetRow();
						if (equipItemTbl == nullptr)
						{
							continue;
						}

						FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(equipItemTbl);
						defenseItemInfoSet->_arrayItem.Add(newData);
					}

					for (auto invadeDefenseEtcItemIter : WorldMapMonsterData.invasionDefenceEtcItemList)
					{
						const FGsSchemaItemCommon* etcItemTbl = invadeDefenseEtcItemIter.GetRow();
						if (etcItemTbl == nullptr)
						{
							continue;
						}

						FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(etcItemTbl);
						defenseItemInfoSet->_arrayItem.Add(newData);
					}


					// add normal item info
					for (auto equipItemIter : WorldMapMonsterData.equipItemList)
					{

						const FGsSchemaItemCommon* equipItemTbl = equipItemIter.GetRow();
						if (equipItemTbl == nullptr)
						{
							continue;
						}

						FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(equipItemTbl);
						defenseItemInfoSet->_arrayItem.Add(newData);
					}

					for (auto etcItemIter : WorldMapMonsterData.etcItemList)
					{
						const FGsSchemaItemCommon* etcItemTbl = etcItemIter.GetRow();
						if (etcItemTbl == nullptr)
						{
							continue;
						}

						FGsRegionMonsterItemInfo* newData = new FGsRegionMonsterItemInfo(etcItemTbl);
						defenseItemInfoSet->_arrayItem.Add(newData);
					}

				}
			}

			newInfo->_arrayMonsterInfo.Add(newMonsterInfo);


			// 몬스터 팝업 레벨 콤보 박스 정보(도감에서 넘어올때 표시용)
			// 
			FGsRegionMonsterSameIDInfo* findInfo = newInfo->_mapMonsterSameIdInfo.FindRef(npcData->id);

			// 기존에 없다면
			if (findInfo == nullptr)
			{			
				findInfo = new FGsRegionMonsterSameIDInfo();
				newInfo->_mapMonsterSameIdInfo.Add(npcData->id, findInfo);

				findInfo->_monsterTblId = npcData->id;
			}

			// 기존에 없던 레벨이면 추가
			if (false == findInfo->_arrayLevel.Contains(monsterLevel))
			{
				findInfo->_arrayLevel.Add(monsterLevel);
			}
		}

		// 몬스터 리스트 정렬
		newInfo->_arrayMonsterInfo.Sort([](FGsRegionMonsterInfo& lhs, FGsRegionMonsterInfo& rhs)
		{
			// 데이터 없으면 리턴
			if (lhs._npcData == nullptr ||
				rhs._npcData == nullptr ||
				lhs._npcBalance == nullptr ||
				rhs._npcBalance == nullptr)
			{
				return true;
			}

			// 1. 필드 보스인지 체크
			// lhs가 필드보스이고 rhs가 아니면 lhs가 앞으로
			if (lhs._fieldBossData != nullptr &&
				rhs._fieldBossData == nullptr)
			{
				return true;
			}
			// 반대면 false
			if (rhs._fieldBossData != nullptr &&
				lhs._fieldBossData == nullptr)
			{
				return false;
			}

			// 둘다 필드 보스일때만 체크
			if (lhs._fieldBossData != nullptr &&
				rhs._fieldBossData != nullptr)
			{
				// 1. lhs 협동, rhs 경쟁이면 lhs 앞으로
				if ((lhs._fieldBossData->bossType == BossType::COOPERATION || lhs._fieldBossData->bossType == BossType::COOPERATION_MUTANT)&&
					(rhs._fieldBossData->bossType == BossType::COMPETITION || rhs._fieldBossData->bossType == BossType::COMPETITION_MUTANT))
				{
					return true;
				}
				// 반대면 false
				if ((rhs._fieldBossData->bossType == BossType::COOPERATION || rhs._fieldBossData->bossType == BossType::COOPERATION_MUTANT) &&
					(lhs._fieldBossData->bossType == BossType::COMPETITION || lhs._fieldBossData->bossType == BossType::COMPETITION_MUTANT))
				{
					return false;
				}
				
				// 1.1 변종이 더 낮음
				if ((lhs._fieldBossData->bossType == BossType::COOPERATION || lhs._fieldBossData->bossType == BossType::COMPETITION) &&
					(rhs._fieldBossData->bossType == BossType::COOPERATION_MUTANT || rhs._fieldBossData->bossType == BossType::COMPETITION_MUTANT))
				{
					return true;
				}

				// 반대면 false
				if ((rhs._fieldBossData->bossType == BossType::COOPERATION || rhs._fieldBossData->bossType == BossType::COMPETITION) &&
					(lhs._fieldBossData->bossType == BossType::COOPERATION_MUTANT || lhs._fieldBossData->bossType == BossType::COMPETITION_MUTANT))
				{
					return false;
				}
			}


			// 2. 등급이 보스면 앞으로
			if (lhs._npcData->grade == CreatureRankType::BOSS &&
				rhs._npcData->grade != CreatureRankType::BOSS)
			{
				return true;
			}

			if (rhs._npcData->grade == CreatureRankType::BOSS &&
				lhs._npcData->grade != CreatureRankType::BOSS)
			{
				return false;
			}
			// 3. 그외는 이름순으로 정렬(이름이 다를때)
			if (lhs._npcData->nameText.EqualTo(rhs._npcData->nameText) == false)
			{
				return  lhs._npcData->nameText.ToString() < rhs._npcData->nameText.ToString();
			}

			// 4. 마지막은 레벨 비교
			return lhs.level < rhs.level;
			
		});

		if(newInfo->_mapMonsterSameIdInfo.Num() != 0)
		{ 
			for (auto monsterSamIdInfo : newInfo->_mapMonsterSameIdInfo)
			{
				// 데이터가 nullptr이면 넘긴다
				if (nullptr == monsterSamIdInfo.Value)
				{
					continue;
				}

				// array값이 0이면 넘긴다
				if (monsterSamIdInfo.Value->_arrayLevel.Num() == 0)
				{
					continue;
				}
				// 레벨 오름차순 정렬하기
				monsterSamIdInfo.Value->_arrayLevel.Sort();
			}
		}

		

		for (FGsRegionMonsterInfo* monsterInfoIter : newInfo->_arrayMonsterInfo)
		{

			for (TPair<EGsRegionItemInfoSetType, FGsRegionMonsterItemInfoSet*> iter : monsterInfoIter->_mapItemInfoSet)
			{
				if (iter.Value == nullptr)
				{
					continue;
				}
				SortItemAll(iter.Value->_arrayItem);
			}
		}

	}

	// 성소 정보 추가
	// https://jira.com2us.com/jira/browse/CHR-23698
	const UGsTable* sanctumDataTbl = FGsSchemaSanctumData::GetStaticTable();
	int totalCount = 0;
	if (nullptr != sanctumDataTbl)
	{
		TArray<const FGsSchemaSanctumData*> rowsSanctumData;
		sanctumDataTbl->GetAllRows<FGsSchemaSanctumData>(rowsSanctumData);

		float sanctumeRegionMapMoveAcceptanceRange = 0.0f;
		if (UGsGlobalConstant* globalData = GData()->GetGlobalData())
		{
			sanctumeRegionMapMoveAcceptanceRange = globalData->_sanctumeRegionMapMoveAcceptanceRange;
		}

		if (rowsSanctumData.Num() != 0)
		{
			for (const FGsSchemaSanctumData* iterSanctumData: rowsSanctumData)
			{
				if (nullptr == iterSanctumData ||
				// 액티브 된거만 사용한다
					iterSanctumData->activeInBattle == false)
				{
					continue;
				}
				const FGsSchemaMapData* sanctumMapData = iterSanctumData->sanctumMapId.GetRow();
				const FGsSchemaNpcData* sanctumNpcData = iterSanctumData->interactionSpawnNpcId.GetRow();

				if (nullptr == sanctumMapData ||
					nullptr == sanctumNpcData)
				{
					continue;
				}
				// map id 가 같으면 추가
				if (In_mapId == sanctumMapData->id)
				{
					const FGsSchemaSpawnData* spawnData =
						GLevel()->GetZone(In_mapId, iterSanctumData->preSpawnGroupId);

					if (spawnData != nullptr && spawnData->SpawnInfoList.Num() != 0)
					{
						
						// 첫번째꺼 쓰기
						FGsSchemaSpawnElement spawnElement = spawnData->SpawnInfoList[0];

						FVector sanctumPos = spawnElement.Pos;
						int32 sanctumdDir = spawnElement.Dir;

						// 회전 값(정면으로)
						FRotator sanctumRotator = FGsDir::AngleToRotator(sanctumdDir);

						FVector sanctumRotVec = sanctumRotator.Vector();

						sanctumRotVec.Normalize();

						// 인터랙션 거리 만큼 띄어놓자
						float sanctumInteractionRange = (float)sanctumNpcData->interactionRange;

						// 역방향으로 해야되는건가
						FVector shifTargetPos = sanctumPos + sanctumRotVec * sanctumInteractionRange;


						FGsRegionMapSanctumInfo* newSanctumInfo = new FGsRegionMapSanctumInfo();

						newSanctumInfo->_sanctumAreaId = iterSanctumData->sanctumAreaId;
						newSanctumInfo->_sanctumName = iterSanctumData->nameText;
						newSanctumInfo->_sanctumPos = shifTargetPos;
						newSanctumInfo->_sanctumAcceptanceRange = sanctumeRegionMapMoveAcceptanceRange;

						newInfo->_arrayRegionMapSanctumInfo.Add(newSanctumInfo);
					}

					
				}
			}
		}		
	}

	// 성소 정보 정렬
	// 갯수가 하나라도 있으면 처리
	// https://jira.com2us.com/jira/browse/CHR-24218
	if (newInfo->_arrayRegionMapSanctumInfo.Num() != 0)
	{
		SortSanctumInfo(newInfo->_arrayRegionMapSanctumInfo);
	}


	return newInfo;
}


// 맵 변경 타입 저장
void FGsGameStateRegionMap::SetMapChangeType(EGsWorldMapChangeType In_type, int In_mapId, unsigned long long In_bookMarkId,
	int In_selectMonsterId)
{
	int showRegionMapId = 0;

	bool isUseMonsterSelectIndex = false;
	// 기본은 씬이다
	EGsRegionMapType showRegionMapType = EGsRegionMapType::Spoils;
	// 보여줄 지역맵 저장
	switch (In_type)
	{
	case EGsWorldMapChangeType::TO_REGIONMAP_CLICK_MINIMAP:
	case EGsWorldMapChangeType::TO_REGIONMAP_MYPOS:
		// 현재 내가 속한 맵정보 출력
		showRegionMapId = GLevel()->GetCurrentLevelId();
		break;
	case EGsWorldMapChangeType::TO_REGIONMAP_AUTOMOVEPOS:
	{
		// 자동 이동 목적지 맵 출력
		FGsMovementHandlerLocalPlayer* movementHandler = GetMovementHandlerLocalPlayer();
		if (movementHandler != nullptr)
		{
			showRegionMapId =
				movementHandler->GetStateAutoMove().GetMoveToMapId();
		}
	}
	break;
	// 대륙맵에서 클릭한 맵
	case EGsWorldMapChangeType::TO_REGIONMAP_CLICK_ITEM:
	// 즐겨찾기에서 클릭한 맵
	case EGsWorldMapChangeType::TO_REGIONMAP_BOOKMARK:		
	case EGsWorldMapChangeType::TO_REGIONMAP_INVASION:
	case EGsWorldMapChangeType::TO_REGIONMAP_DROP:
		showRegionMapId = In_mapId;
		break;
	case EGsWorldMapChangeType::TO_REGIONMAP_DUNGEON_MONSTERINF0:
	case EGsWorldMapChangeType::TO_REGIONMAP_MonsterInfoPopup:
		// 던전 ui 에서 해당맵 몬스터 정보 보기
		showRegionMapId = In_mapId;
		showRegionMapType = EGsRegionMapType::Monster;
		if (In_selectMonsterId != 0)
		{
			isUseMonsterSelectIndex = true;
			int findIndex = FindMonsterIndex(In_mapId, In_selectMonsterId);
			int selIndex = (findIndex == -1) ? 0 : findIndex;
			GSWorldMap()->SetBackupSelMonsterListIndex(selIndex);
		}
		break;
		// 최근걸로 다시 세팅
	case EGsWorldMapChangeType::TO_REGIONMAP_OLD_REGIONMAP:
		showRegionMapId = GSWorldMap()->GetShowRegionMapId();
		break;
	}

	if (showRegionMapId == 0)
	{
		return;
	}

	GSWorldMap()->SetShowRegionMapId(showRegionMapId);
	GSWorldMap()->SetShowRegionMapType(showRegionMapType);

	FGsRegionInfo* currentRegionInfo = FindRegionInfo(showRegionMapId);
	
	GSWorldMap()->SetCurrentRegionInfo(currentRegionInfo);

	GSWorldMap()->LoadRegionData(showRegionMapId);

	GSWorldMap()->SetBookmarkSelectId(In_bookMarkId);

	GSWorldMap()->SetIsUseMonsterSelectIndex(isUseMonsterSelectIndex);
}

FGsMovementHandlerLocalPlayer* FGsGameStateRegionMap::GetMovementHandlerLocalPlayer()
{
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return nullptr;
	}

	UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == local)
	{
		return nullptr;
	}

	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	return movement;
}

// 포털로 이동
void FGsGameStateRegionMap::MoveToPortal(int In_index)
{
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	int currentMapId = GLevel()->GetCurrentLevelId();

	if (showRegionMapId != currentMapId)
	{
		if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MOVE_TO_OTHER_REGION, true))
		{
			return;
		}
	}
	
	FGsPortalInfo* portalInfo =
		GSWorldMap()->GetCurrentRegionPortalInfoByIndex(In_index);

	if (portalInfo == nullptr)
	{
		return;
	}

	GSAI()->MoveToPosAuto(EGsMovementAutoContentsType::RegionMapPortalMove, showRegionMapId,
		portalInfo->_portalPos);
}

// 지역맵 이동과 이동 팝업 띄우기(return false면 이동 못하는곳임)
bool FGsGameStateRegionMap::ShowRegionMapAndMovePopup(int In_mapId)
{
	// 언락 컨텐츠와 가능 타입 맵, 관전 모드 체크 한번에 하자
	if (FGsWorldMapFunc::IsRegionMapOpenPossible() == false)
	{
		return false;
	}

	// 기존에 만든 정보가 없으면 
	FGsRegionInfo* regionInfo = FindRegionInfo(In_mapId);

	if (regionInfo == nullptr ||
		regionInfo->_arrayPortalInfo.Num() == 0)
	{
		return false;
	}
	// 첫번째 포털
	int portalIndex = 0;
	
	FGsPortalInfo* portalInfo = regionInfo->_arrayPortalInfo[portalIndex];

	if (portalInfo == nullptr)
	{
		return false;
	}
	// region 맵세팅
	SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_CLICK_ITEM, In_mapId);
	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
	{
		return false;
	}
	if (uiManager->IsActiveWidget(TEXT("WindowRegionMap")))
	{
		// 지역맵이 켜져있다면 변경
		GMessage()->GetUI().SendMessage(MessageUI::CHANGE_REGION_MAP_SCENE, nullptr);
	}
	else
	{
		// 꺼져 있다면 지역맵 열기
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
	}



	return true;
}

// 크리쳐맵 띄우기
bool FGsGameStateRegionMap::ShowCreatureMap(const MapId InMapId, const CreatureId InCreatureId)
{

	// 언락 컨텐츠와 가능 타입 맵, 관전 모드 체크 한번에 하자
	if (FGsWorldMapFunc::IsRegionMapOpenPossible() == false)
	{
		return false;
	}

	// 기존에 만든 정보가 없으면 
	FGsRegionInfo* regionInfo = FindRegionInfo(InMapId);
	
	// region 맵세팅
	SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_DUNGEON_MONSTERINF0, InMapId, 0, InCreatureId);

	// 크리쳐맵으로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);

	return true;
}

// 몬스터 팝업과 지도 열기
bool FGsGameStateRegionMap::ShowMonsterInfoPopupAndMap( MapId InMapId,  CreatureId InCreatureId, bool In_isRegionWindowOpen)
{
	// 언락 컨텐츠와 가능 타입 맵, 관전 모드 체크 한번에 하자
	if (FGsWorldMapFunc::IsRegionMapOpenPossible() == false ||
		GSWorldMap() == nullptr || 
		GUI() == nullptr)
	{
		return false;
	}

	// 기존에 만든 정보가 없으면 
	FGsRegionInfo* regionInfo = FindRegionInfo(InMapId);
	if (regionInfo == nullptr)
	{
		return false;
	}

	int selectIndex = -1;
	if(In_isRegionWindowOpen == true)
	{ 

		// region 맵세팅
		SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_MonsterInfoPopup, InMapId, 0, InCreatureId);

		// 크리쳐맵으로 이동
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);

		// 팝업 열기
		selectIndex = GSWorldMap()->GetBackupSelMonsterListIndex();
	}
	else
	{
		// 팝업만 열꺼면 순서 찾는거 별도로 처리
		int findIndex = FindMonsterIndex(InMapId, InCreatureId);
		selectIndex = (findIndex == -1) ? 0 : findIndex;

		GSWorldMap()->SetCurrentRegionInfo(regionInfo);

		// 침공 수비일때 도감 통해 몬스터 팝업 볼때 보상 정보 안나오는 현상
		// 현재 보고 있는 맵 id  정보가 저장이 안되었다(현재 보는 맵 id로 침공 방어인가 체크함)
		// https://jira.com2us.com/jira/browse/C2URWQ-7461
		GSWorldMap()->SetShowRegionMapId(InMapId);
	}
	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->OpenAndGetWidget(TEXT("PopupMonsterInfo"));

	if (wigdet.IsValid() == false)
	{
		return false;
	}

	// 선택된 정보 저장해놓기
	// 선택된 정보 가지고 콤보 박스 세팅함(먼저 저장해놓아야 함)
	GSWorldMap()->SetSelectMonsterInfoByIndex(selectIndex);

	TWeakObjectPtr<UGsUIPopupMonsterInfo> popupMonsgerInfo = Cast<UGsUIPopupMonsterInfo>(wigdet);
	popupMonsgerInfo->InitMonsterInfoList(selectIndex, true);


	return true;
}

// 드랍 아이템 맵 띄우기
bool FGsGameStateRegionMap::ShowDropItemMap(const MapId InMapId, const CreatureId InCreatureId, const Level InLevel,  EGsToggleType inToggleType)
{
	// 언락 컨텐츠와 가능 타입 맵, 관전 모드 체크 한번에 하자
	if (FGsWorldMapFunc::IsRegionMapOpenPossible() == false)
	{
		return false;
	}

	// 기존에 만든 정보가 없으면 
	FGsRegionInfo* regionInfo = FindRegionInfo(InMapId);

	// region 맵세팅
	SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_DROP, InMapId, 0, InCreatureId);

	
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsContentsManagerGame* contentsManagerGame = gameFlow->GetContentsManagerGame())
		{
			// 만약 이미 RegionMap Window 가 띄워져 있는 상태라면... 탭을 지도 탭으로 변경한다
			if (contentsManagerGame->GetCurrentContentsMode() == FGsContentsMode::ContentsRegionMap)
			{
				FGsUIMsgRegionMapTypeParam sendParam(EGsRegionMapType::Spoils);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_CLICK_REGION_MAP_TYPE_TAB, &sendParam);
			}
			// RegionMap Window 가 안 띄워져 있으면 드랍 아이템 맵으로 이동
			else
			{
				GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
			}
		}
	}

	//// 테이블에서 CreatureId 로 마커 찍을 위치 갖고 오기
	//// MapId 와 SpawnGroupId 도 추가로 전달해 주어야, 나중에 마커가 텔레포트 팝업을 바로 띄울 수 있다
	//TArray<SpawnGroupId> spawnGroupIdArray;
	//TArray<FVector> posArray;
	//	
	//SetMarkerOfMonster(InMapId, InCreatureId, InLevel, spawnGroupIdArray, posArray);

	//// 만약 스폰 정보가 제대로 입력되지 않았을 경우에는, 마커를 띄워주지 않고 대신 RegionMap 만 띄워준다
	//if (posArray.Num() > 0)
	//{
	//	FGsMapMonsterDropIconParam param(InMapId, InCreatureId, spawnGroupIdArray, posArray);

	//	// 마커 띄워주기
	//	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ICON, &param);

	//	// 맵 우측 버특 액티브 on
	//	FGsUIMsgActiveParam activeParam = FGsUIMsgActiveParam(true);
	//	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ACTIVE, &activeParam);

	//	// 마커를 띄워줄때, MonsterDrop Icon 만 필터링 된 상태로 시작해야 함
	//	// 현재  ShowDropItemMap inToggleType 는 다 EGsToggleType::Active로 호출되고 있음
	//	FGsUIMsgToggleParam toggleParam = FGsUIMsgToggleParam(inToggleType);
	//	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ICON_TOGGLE, &toggleParam);
	//}

	//SelectSpoilsMonster(InMapId, InCreatureId, InLevel);

	ShowSpoilIconAndFilterButtonOn(InMapId, InCreatureId, InLevel, inToggleType);

	return true;
}

void FGsGameStateRegionMap::SetMarkerOfMonster(const MapId InMapId, const CreatureId InCreatureId, const Level InLevel, TArray<SpawnGroupId>& InSpawnGroupIdArray, OUT TArray<FVector>& InPosArray)
{
	InSpawnGroupIdArray.Empty();
	InPosArray.Empty();
	
	const UGsTableMonsterToMapData* table = Cast<UGsTableMonsterToMapData>(FGsSchemaMonsterToMapData::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaMonsterToMapData* monsterToMapData;
	FName rowName = FName(*FString::Format(TEXT("{0}_{1}"), { FString::FromInt(InCreatureId), FString::FromInt(InLevel) }));
	table->FindRow(rowName, monsterToMapData);

	if (nullptr == monsterToMapData)
	{
		GSLOG(Error, TEXT("nullptr == monsterToMapData"));
		return;
	}
		

	TArray<FGsSchemaMonsterInMapData> monsterInMapDataList = monsterToMapData->MonsterInMapDataList;
	for (const FGsSchemaMonsterInMapData monsterInMapData : monsterInMapDataList)
	{
		if (InMapId == monsterInMapData.MapId)
		{
			TArray<FGsSchemaMonsterInRegionData> monsterInRegionDataList = monsterInMapData.MonsterInRegionDataList;
			for (FGsSchemaMonsterInRegionData regionData : monsterInRegionDataList)
			{
				const FGsSchemaSpawnData* spawnData = regionData.SpawnGroupId.GetRow();
				InSpawnGroupIdArray.Add(spawnData->SpawnGroupId);
				InPosArray.Add(spawnData->Region.Center);
			}
		}
	}
}

void FGsGameStateRegionMap::OnChangeRegionMapScene(const IGsMessageParam*)
{
	if (_windowUI.IsValid())
	{
		// 원래는 Scene(포탈) 이었는데 월드맵 개선으로 인해 전리품으로 변경
		// https://jira.com2us.com/jira/browse/CHR-23756
		_windowUI->ChangeRegionMapByType(EGsRegionMapType::Spoils);
	}
}

// find region map info
// create infomation if it doesn't exist
FGsRegionInfo* FGsGameStateRegionMap::FindRegionInfo(int In_mapId)
{
	FGsRegionInfo* findRegionInfo = GSWorldMap()->FindRegionInfo(In_mapId);
	if (findRegionInfo == nullptr)
	{
		findRegionInfo = MakeRegionInfo(In_mapId);
		GSWorldMap()->SetRegionInfo(In_mapId, findRegionInfo);
	}

	return findRegionInfo;
}

bool FGsGameStateRegionMap::IsInvadePossibleMap(int In_mapId)
{
	const UGsTableInvadeMapData* table = Cast<UGsTableInvadeMapData>(FGsSchemaInvadeMapData::GetStaticTable());
	if (nullptr == table)
	{
		return false;
	}
	TArray<const FGsSchemaInvadeMapData*> arrayData;
	table->GetAllRows<FGsSchemaInvadeMapData>(arrayData);

	bool isInvadePossibleMap = false;

	for (const FGsSchemaInvadeMapData* iter : arrayData)
	{
		if (nullptr == iter)
		{
			continue;
		}
		if (nullptr == iter->mapId.GetRow())
		{
			continue;
		}
		const FGsSchemaMapData* mapData = iter->mapId.GetRow();
		if (mapData == nullptr)
		{
			continue;
		}
		if (mapData->id == In_mapId)
		{
			isInvadePossibleMap = true;
			break;
		}
	}

	return isInvadePossibleMap;
}


void FGsGameStateRegionMap::SortEquipItem(TArray< FGsRegionMonsterItemInfo*>& In_sortTarget)
{
	if (In_sortTarget.Num() == 0)
	{
		return;
	}

	// 아이템 정렬(등급이 높으면 앞으로 감)
	In_sortTarget.Sort([](FGsRegionMonsterItemInfo& lhs, FGsRegionMonsterItemInfo& rhs)
	{
		const FGsSchemaItemCommon* leftItemTbl = lhs._itemTbl;
		const FGsSchemaItemCommon* rightItemTbl = rhs._itemTbl;

		if (leftItemTbl != nullptr &&
			rightItemTbl == nullptr)
		{
			return true;
		}
		// 반대면 false
		if (rightItemTbl != nullptr &&
			leftItemTbl == nullptr)
		{
			return false;
		}

		// 1. grade diff(higher grade go to front)
		int leftGrade = (int)leftItemTbl->grade;
		int rightGrade = (int)rightItemTbl->grade;
		if (leftGrade != rightGrade)
		{
			return leftGrade > rightGrade;
		}
		// 2. tbl id(lower id go to front)
		return leftItemTbl->id < rightItemTbl->id;
	});
}
void FGsGameStateRegionMap::SortEtcItem(TArray< FGsRegionMonsterItemInfo*>& In_sortTarget)
{
	if (In_sortTarget.Num() == 0)
	{
		return;
	}

	In_sortTarget.Sort([](FGsRegionMonsterItemInfo& lhs, FGsRegionMonsterItemInfo& rhs)
	{
		const FGsSchemaItemCommon* leftItemTbl = lhs._itemTbl;
		const FGsSchemaItemCommon* rightItemTbl = rhs._itemTbl;

		if (leftItemTbl != nullptr &&
			rightItemTbl == nullptr)
		{
			return true;
		}
		// 반대면 false
		if (rightItemTbl != nullptr &&
			leftItemTbl == nullptr)
		{
			return false;
		}

		// 1. grade diff(higher grade go to front)
		int leftGrade = (int)leftItemTbl->grade;
		int rightGrade = (int)rightItemTbl->grade;
		if (leftGrade != rightGrade)
		{
			return (int)leftGrade > (int)rightGrade;
		}
		// 2. category main diff(lower type go to front)
		FGsRegionMonterItemInfoEtc* leftEtcInfo = static_cast<FGsRegionMonterItemInfoEtc*>(&lhs);
		FGsRegionMonterItemInfoEtc* rightEtcInfo = static_cast<FGsRegionMonterItemInfoEtc*>(&rhs);

		if (leftEtcInfo != nullptr &&
			rightEtcInfo == nullptr)
		{
			return true;
		}
		// 반대면 false
		if (rightEtcInfo != nullptr &&
			leftEtcInfo == nullptr)
		{
			return false;
		}

		if (leftEtcInfo->_sortType != rightEtcInfo->_sortType)
		{
			return (int)leftEtcInfo->_sortType < (int)rightEtcInfo->_sortType;
		}

		// 3. tbl id(lower id go to front)
		return leftItemTbl->id < rightItemTbl->id;
	});
}

int FGsGameStateRegionMap::FindMonsterIndex(int In_mapId, int In_monsterId)
{
	FGsRegionInfo* currentRegionInfo = FindRegionInfo(In_mapId);
	if (currentRegionInfo == nullptr)
	{
		return -1;
	}

	for ( int i =0;  currentRegionInfo->_arrayMonsterInfo.Num() > i; ++i)
	{
		FGsRegionMonsterInfo* iterMonsterInfo = currentRegionInfo->_arrayMonsterInfo[i];
		if (iterMonsterInfo == nullptr)
		{
			continue;
		}
		const FGsSchemaNpcData* npcData = iterMonsterInfo->_npcData;
		if (npcData == nullptr)
		{
			continue;
		}
		
		if (npcData->id == In_monsterId)
		{
			return i;
		}
	}

	return -1;
}
// map id와 몬스터 id와 레벨로 지역맵 데이터 몬스터 인덱스 구하기
int FGsGameStateRegionMap::FindMonsterIndexByIdAndLevel(int In_mapId, int In_monsterId, int In_monsterLevel)
{
	FGsRegionInfo* currentRegionInfo = FindRegionInfo(In_mapId);
	if (currentRegionInfo == nullptr)
	{
		return -1;
	}

	for (int i = 0; currentRegionInfo->_arrayMonsterInfo.Num() > i; ++i)
	{
		FGsRegionMonsterInfo* iterMonsterInfo = currentRegionInfo->_arrayMonsterInfo[i];
		if (iterMonsterInfo == nullptr)
		{
			continue;
		}
		const FGsSchemaNpcData* npcData = iterMonsterInfo->_npcData;
		if (npcData == nullptr)
		{
			continue;
		}
		// id와 레벨이 같다면
		if (npcData->id == In_monsterId && 
			iterMonsterInfo->level == In_monsterLevel)
		{
			return i;
		}
	}

	return -1;
}

void FGsGameStateRegionMap::SelectSpoils(const FGsSchemaWorldMapSpoilsItem& inSpoils)
{
	_selectSpoils.Set(inSpoils);
}

void FGsGameStateRegionMap::ClearSpoils()
{
	_selectSpoils.Clear();
}

bool FGsGameStateRegionMap::IsEmptySpoils()
{
	return false == _selectSpoils.IsValid();
}

bool FGsGameStateRegionMap::IsEqualsSpoils(const FGsSchemaItemCommon* inItem)
{
	return _selectSpoils.spoilsItem.item.GetRow() == inItem;
}

void FGsGameStateRegionMap::SelectSpoilsMonster(MapId inMapId, CreatureId inCreatureId, Level inLevel)
{
	_selectSpoilsMonster.Set(inMapId, inCreatureId, inLevel);
}

void FGsGameStateRegionMap::ClearSpoilsMonster()
{
	_selectSpoilsMonster.Clear();
}

bool FGsGameStateRegionMap::IsEmptySpoilsMonster()
{
	return false == _selectSpoilsMonster.IsValid();
}

bool FGsGameStateRegionMap::TryGetSpoilsMonster(OUT FGsSpoilsMonster& outSpoilsMonster)
{
	if (_selectSpoilsMonster.IsValid())
	{
		outSpoilsMonster = _selectSpoilsMonster;
		return true;
	}

	return false;
}

bool FGsGameStateRegionMap::IsSpoilsMonsterExist(MapId inMapId)
{
	return _selectSpoilsMonster.mapId == inMapId;
}
// 침공전 포털로 이동(지역맵에서)
// 몬스터 드랍 위치로 이동
void FGsGameStateRegionMap::MoveToPortalPos(FVector In_pos, float In_useAcceptanceRadius)
{
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	int currentMapId = GLevel()->GetCurrentLevelId();

	if (showRegionMapId != currentMapId)
	{
		if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MOVE_TO_OTHER_REGION, true))
		{
			return;
		}
	}


	GSAI()->MoveToPosAuto(EGsMovementAutoContentsType::RegionMapPortalMove, showRegionMapId,
		In_pos, true,In_useAcceptanceRadius);
}

// 1. 몬스터 위치(획득처) 아이콘 표시
// 2. 필터링 버튼(우측 버튼) 상태 on
// 현재ShowDropItemMap 함수는 지도 열어서 획득처 탭변경 까지 하는데
// 지도내에 몬스터 리스트 클릭시에도 출력해야되어서 별도 함수 생성
// https://jira.com2us.com/jira/browse/CHR-23756
void FGsGameStateRegionMap::ShowSpoilIconAndFilterButtonOn(MapId In_mapId, CreatureId In_creatureId, Level In_level,
	EGsToggleType In_toggleType)
{
	// 테이블에서 CreatureId 로 마커 찍을 위치 갖고 오기
	// MapId 와 SpawnGroupId 도 추가로 전달해 주어야, 나중에 마커가 텔레포트 팝업을 바로 띄울 수 있다
	TArray<SpawnGroupId> spawnGroupIdArray;
	TArray<FVector> posArray;

	SetMarkerOfMonster(In_mapId, In_creatureId, In_level, spawnGroupIdArray, posArray);

	if (posArray.Num() > 0)
	{
		FGsMapMonsterDropIconParam param(In_mapId, In_creatureId, spawnGroupIdArray, posArray);

		// 마커 띄워주기
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ICON, &param);

		// 맵 우측 버특 액티브 on
		FGsUIMsgActiveParam activeParam = FGsUIMsgActiveParam(true);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ACTIVE, &activeParam);

		// 마커를 띄워줄때, MonsterDrop Icon 만 필터링 된 상태로 시작해야 함
		// 현재  ShowDropItemMap inToggleType 는 다 EGsToggleType::Active로 호출되고 있음
		FGsUIMsgToggleParam toggleParam = FGsUIMsgToggleParam(In_toggleType);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ICON_TOGGLE, &toggleParam);
	}

	SelectSpoilsMonster(In_mapId, In_creatureId, In_level);
}

// 몬스터 드랍 필터 갱신
// https://jira.com2us.com/jira/browse/CHR-23756
void FGsGameStateRegionMap::InvalidateMonsterDropFilter()
{
	//현재 선택한 맵에 이전에 선택한 전리품과 이전에 선택한 전리품 드랍하는 몬스터 정보가 있다면 전리품 정보 보여주기
	//아니라면 전리품 버튼 비활성화 
	FGsUIMsgToggleParam toggleParam = FGsUIMsgToggleParam(EGsToggleType::Deactive);
	if ((false == IsEmptySpoilsMonster() && false == IsEmptySpoils())
		&& IsSpoilsMonsterExist(GSWorldMap()->GetShowRegionMapId()))
	{
		toggleParam = FGsUIMsgToggleParam(EGsToggleType::Active);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ICON_TOGGLE, &toggleParam);

		FGsUIMsgActiveParam activeParam = FGsUIMsgActiveParam(true);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ACTIVE, &activeParam);
	}
	else
	{
		toggleParam = FGsUIMsgToggleParam(EGsToggleType::Deactive);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ICON_TOGGLE, &toggleParam);

		FGsUIMsgActiveParam activeParam = FGsUIMsgActiveParam(false);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ACTIVE, &activeParam);
	}
}

// 월드맵 개선 
// 드랍 아이템 통합 출력
// https://jira.com2us.com/jira/browse/CHR-23756
void FGsGameStateRegionMap::SortItemAll(TArray< FGsRegionMonsterItemInfo*>& In_sortTarget)
{

	if (In_sortTarget.Num() == 0)
	{
		return;
	}

	// 월드맵 개선(드랍템 장비, etc 통합)
	// 1. 등급
	// 2. 아이템 메인 분류(Weapon-Armor-Accessary-Skillbook-Else)
	// 3. 아이템 id 순(오름 차순)

	In_sortTarget.Sort([](FGsRegionMonsterItemInfo& lhs, FGsRegionMonsterItemInfo& rhs)
	{
		const FGsSchemaItemCommon* leftItemTbl = lhs._itemTbl;
		const FGsSchemaItemCommon* rightItemTbl = rhs._itemTbl;

		if (leftItemTbl != nullptr &&
			rightItemTbl == nullptr)
		{
			return true;
		}
		// 반대면 false
		if (rightItemTbl != nullptr &&
			leftItemTbl == nullptr)
		{
			return false;
		}

		// 1. 등급 다를때 diff(higher grade go to front)
		int leftGrade = (int)leftItemTbl->grade;
		int rightGrade = (int)rightItemTbl->grade;
		if (leftGrade != rightGrade)
		{
			return (int)leftGrade > (int)rightGrade;
		}
		// 2. 정렬 카테고리 다를때 (lower type go to front)

		if (lhs._itemSortType != rhs._itemSortType)
		{
			return (int)lhs._itemSortType < (int)rhs._itemSortType;
		}

		// 3. 테이블 id(lower id go to front)
		return leftItemTbl->id < rightItemTbl->id;
	});
}
// 성소 정보 정렬
// id 오름 차순
// https://jira.com2us.com/jira/browse/CHR-24218
void FGsGameStateRegionMap::SortSanctumInfo(TArray<FGsRegionMapSanctumInfo*>& In_sortTarget)
{
	if (In_sortTarget.Num() == 0)
	{
		return;
	}

	// 월드맵 개선(드랍템 장비, etc 통합)
	// 1. 아이템 id 순(오름 차순)

	In_sortTarget.Sort([](FGsRegionMapSanctumInfo& lhs, FGsRegionMapSanctumInfo& rhs)
		{
			// 1. 테이블 id(lower id go to front)
			return lhs._sanctumAreaId < rhs._sanctumAreaId;
		});
}
// 몬스터 아이템 검색할 타입 찾기(일반, 침공 공격, 침공 수비)
EGsRegionItemInfoSetType FGsGameStateRegionMap::GetNowMapItemInfoSetType(FGsRegionMonsterInfo* In_monsterInfo)
{
	if (In_monsterInfo == nullptr ||
		In_monsterInfo->_npcData == nullptr)
	{
		return EGsRegionItemInfoSetType::Normal;
	}

	// field boss only use normal
	if (In_monsterInfo->_npcData->npcFunctionType == NpcFunctionType::FIELD_BOSS)
	{
		return EGsRegionItemInfoSetType::Normal;
	}

	// 1. check is invade
	if (true == GGameData()->IsInvadeWorld())
	{
		return EGsRegionItemInfoSetType::Invade_Attack;
	}


	// 2. check is defense map
	// 3. etc = normal
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	bool isDefense = GSInterServer()->IsValidInvasionMap(showRegionMapId);


	return (isDefense == true) ? EGsRegionItemInfoSetType::Invade_Defense : EGsRegionItemInfoSetType::Normal;
}