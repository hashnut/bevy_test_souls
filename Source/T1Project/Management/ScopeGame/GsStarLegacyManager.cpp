#include "GsStarLegacyManager.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacySlotArrangementInfo.h"
#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacyCategoryInfo.h"
#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacySlotMagnitudeInfo.h"
#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacySlotItemInfo.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateStarLegacy.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"

#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacyCategory.h"
#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacySlot.h"
#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacySlotSpawn.h"
#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacyConfig.h"
#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacyIdentifyChance.h"

#include "UTIL/GsUIUtil.h"
#include "UTIL/GsTableUtil.h"

#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"

#include "Runtime/Core/Public/Misc/Timespan.h"

// 테이블 데이터 가공
void FGsStarLegacyManager::ProcessTableData()
{
	// 카테고리 데이터
	const UGsTable* starLegacyCategoryTbl = FGsSchemaStarLegacyCategory::GetStaticTable();
	if (nullptr == starLegacyCategoryTbl)
	{
		return;
	}

	TArray<const FGsSchemaStarLegacyCategory*> rowsStarLegacyCategoryData;
	starLegacyCategoryTbl->GetAllRows<FGsSchemaStarLegacyCategory>(rowsStarLegacyCategoryData);

	if (rowsStarLegacyCategoryData.Num() == 0)
	{
		return;
	}



	for (const FGsSchemaStarLegacyCategory* categoryIter: rowsStarLegacyCategoryData)
	{
		if (nullptr == categoryIter)
		{
			continue;
		}

		int categoryId = categoryIter->id;

		_arrayCategoryIds.Add(categoryId);

		FGsStarLegacyCategoryInfo** findInfo = _mapCategoryInfo.Find(categoryId);

		// 기존에 없다면
		if (findInfo == nullptr)
		{
			// 카테고리 id로 슬롯 정보 가져오기
			TArray<const FGsSchemaStarLegacySlot*> slotTbls = FindSlotTbl(categoryId);
			if (slotTbls.Num() == 0)
			{
				continue;
			}

			FGsStarLegacyCategoryInfo* newCategoryData = new FGsStarLegacyCategoryInfo();

			// id 저장
			newCategoryData->_categoryId = categoryId;

			// 테이블 저장
			newCategoryData->_tblInfo = categoryIter;


			for (const FGsSchemaStarLegacySlot* slotIter : slotTbls)
			{
				if (slotIter == nullptr)
				{
					continue;
				}

				int slotId = slotIter->legacySlotId;

				// 보상 아이템 정보 세팅
				TArray<const FGsSchemaStarLegacySlotSpawn*> rewardTbls = FindSlotRewardTbl(slotId);
				if (rewardTbls.Num() == 0)
				{
					continue;
				}

				FGsStarLegacySlotMagnitudeInfo* newMagnitudeData = new  FGsStarLegacySlotMagnitudeInfo();
				// 슬롯 id
				newMagnitudeData->_slotId = slotId;
				// 텍스트 이름(5등성)
				newMagnitudeData->_nameText = slotIter->nameText;
				
				// 1000배율 백분율로 바꾸기
				newMagnitudeData->SetPercentageByThousandMagnification(slotIter->rouletteRate);

				// 보상 정보 세팅
				for (const FGsSchemaStarLegacySlotSpawn* rewardIter: rewardTbls)
				{
					if (rewardIter == nullptr)
					{
						continue;
					}

					FGsStarLegacySlotItemInfo* newItemData = new FGsStarLegacySlotItemInfo();
					// 보상 아이템 등급
					newItemData->_itemGrade = rewardIter->itemGrade;
					// 보상 텍스트 
					newItemData->_nameText = rewardIter->nameText;
					// 보상 id 
					newItemData->_rewardDataId = rewardIter->rewardDataId;
					// 천배율에서 백분율
					newItemData->_acquisitionProbabilityPercentage = UGsUIUtil::GetPercentageByThousandMagnification(rewardIter->rewardSpawnRate);
					

					newMagnitudeData->_arraySlotItemInfo.Add(newItemData);
				}
				// 보상 정보 정렬(보상 tbl id 오름차순)
				newMagnitudeData->_arraySlotItemInfo.Sort([](FGsStarLegacySlotItemInfo& lhs, FGsStarLegacySlotItemInfo& rhs)
				{
					return lhs._rewardDataId < rhs._rewardDataId;
				});


				// 등급정보()
				newCategoryData->_arraySlotMagnitudeInfo.Add(newMagnitudeData);
			}

			// 슬롯 id 로 정렬(오름 차순)
			newCategoryData->_arraySlotMagnitudeInfo.Sort([](FGsStarLegacySlotMagnitudeInfo& lhs, FGsStarLegacySlotMagnitudeInfo& rhs)
			{
				return lhs._slotId < rhs._slotId;
			});


			// 카테고리 맵데이터에 넣기
			_mapCategoryInfo.Add(categoryId, newCategoryData);
		}

	}

	const FGsSchemaStarLegacyConfig* starLegacyConfigTbl = FindStarLegacyConfig();
	if (starLegacyConfigTbl == nullptr)
	{
		return;
	}

	if (_arrayTimeTickWaitingTimeToPass.Num() != 0)
	{
		_arrayTimeTickWaitingTimeToPass.Empty();
	}
	//  한칸 넘어갈때 대기 tick 시간
	for (float iter : starLegacyConfigTbl->arrayWaitingTimeToPass)
	{
		_arrayTimeTickWaitingTimeToPass.Add(iter* ETimespan::TicksPerSecond);
	}


	// 룰렛 회전 횟수
	_numberOfRouletteSpins = starLegacyConfigTbl->numberOfRouletteSpins;
}

// 카테고리 id로 슬롯 array 테이블 값 가져오기
TArray<const FGsSchemaStarLegacySlot*> FGsStarLegacyManager::FindSlotTbl(int In_categoryId)
{
	TArray<const FGsSchemaStarLegacySlot*> findTbls;

	// 슬롯 데이터
	const UGsTable* starLegacySlotTbl = FGsSchemaStarLegacySlot::GetStaticTable();
	if (nullptr == starLegacySlotTbl)
	{
		return findTbls;
	}

	TArray<const FGsSchemaStarLegacySlot*> rowsStarLegacySlotData;
	starLegacySlotTbl->GetAllRows<FGsSchemaStarLegacySlot>(rowsStarLegacySlotData);

	if (rowsStarLegacySlotData.Num() == 0)
	{
		return findTbls;
	}


	for (const FGsSchemaStarLegacySlot* iter : rowsStarLegacySlotData)
	{
		if (nullptr == iter)
		{
			continue;
		}

		if (iter->legacyCategoryId == In_categoryId)
		{
			findTbls.Add(iter);
		}
	}

	return findTbls;
}

// 슬롯 id로 아이템 스폰 정보 array 테이블 값 가저오기
TArray<const FGsSchemaStarLegacySlotSpawn*> FGsStarLegacyManager::FindSlotRewardTbl(int In_slotId)
{
	TArray<const FGsSchemaStarLegacySlotSpawn*> findTbls;

	// 슬롯 데이터
	const UGsTable* starLegacySlotSpawnTbl = FGsSchemaStarLegacySlotSpawn::GetStaticTable();
	if (nullptr == starLegacySlotSpawnTbl)
	{
		return findTbls;
	}

	TArray<const FGsSchemaStarLegacySlotSpawn*> rowsStarLegacySlotSpawnData;
	starLegacySlotSpawnTbl->GetAllRows<FGsSchemaStarLegacySlotSpawn>(rowsStarLegacySlotSpawnData);

	if (rowsStarLegacySlotSpawnData.Num() == 0)
	{
		return findTbls;
	}


	for (const FGsSchemaStarLegacySlotSpawn* iter : rowsStarLegacySlotSpawnData)
	{
		if (nullptr == iter)
		{
			continue;
		}

		if (iter->legacySlotId == In_slotId)
		{
			findTbls.Add(iter);
		}
	}

	return findTbls;
}
// 초기화
void FGsStarLegacyManager::Initialize()
{
	// 데이터 가공
	ProcessTableData();
}

// 슬롯 정보 클리어
void FGsStarLegacyManager::ClearAllSlotArrangeInfo()
{
	if (_mapSlotArrangInfo.Num() != 0)
	{
		for (auto categorySlotArrangeInfo : _mapSlotArrangInfo)
		{
			if (0 == categorySlotArrangeInfo.Value.Num())
			{
				continue;
			}

			for (FGsStarLegacySlotArrangementInfo* arrangeInfoIter : categorySlotArrangeInfo.Value)
			{
				if (arrangeInfoIter == nullptr)
				{
					continue;
				}

				delete arrangeInfoIter;
			}
			categorySlotArrangeInfo.Value.Empty();
		}

		_mapSlotArrangInfo.Empty();
	}

	if (_mapCategoryServerPickSlotId.Num() != 0)
	{
		_mapCategoryServerPickSlotId.Empty();
	}

}
// 슬롯 정보 카테고리당 클리어
void FGsStarLegacyManager::ClearSlotArrangeInfoByCategory(int In_categoryId)
{
	TArray<FGsStarLegacySlotArrangementInfo*>* findCategoryInfo = _mapSlotArrangInfo.Find(In_categoryId);
	// 카테고리 정보가 있고 배열이 0이 아니면 
	if (findCategoryInfo != nullptr && findCategoryInfo->Num() != 0)
	{
		for (FGsStarLegacySlotArrangementInfo* arrangeInfoIter: *findCategoryInfo)
		{
			if (arrangeInfoIter == nullptr)
			{
				continue;
			}

			delete arrangeInfoIter;
		}
		findCategoryInfo->Empty();

		_mapSlotArrangInfo.Remove(In_categoryId);
	}	

	if(_mapCategoryServerPickSlotId.Contains(In_categoryId) == true)
	{ 
		_mapCategoryServerPickSlotId.Remove(In_categoryId);
	}
}

void FGsStarLegacyManager::Finalize()
{
	ClearAllSlotArrangeInfo();

	if (_mapCategoryInfo.Num() != 0)
	{
		for (auto categoryIter : _mapCategoryInfo)
		{
			if (nullptr == categoryIter.Value)
			{
				continue;
			}

			categoryIter.Value->Finalize();
			delete categoryIter.Value;
		}
		_mapCategoryInfo.Empty();
	}

	if (_arrayCategoryIds.Num() != 0)
	{
		_arrayCategoryIds.Empty();
	}
}
// 테스트 배치 데이터 만들기
void FGsStarLegacyManager::MakeTestSlotArrangementInfo()
{
	// 카테고리 갯수 만큼 만듬
	const UGsTable* catetoryTbl = FGsSchemaStarLegacyCategory::GetStaticTable();
	if (nullptr == catetoryTbl)
	{
		return;
	}

	TArray<const FGsSchemaStarLegacyCategory*> rowsCategoryData;
	catetoryTbl->GetAllRows<FGsSchemaStarLegacyCategory>(rowsCategoryData);
	int catogoryCount = rowsCategoryData.Num();
	if (catogoryCount == 0)
	{
		return;
	}


	for (auto categoryIter : rowsCategoryData )
	{
		if (categoryIter == nullptr)
		{
			continue;
		}
		int categoryId = categoryIter->id;

		TArray<FGsStarLegacySlotArrangementInfo*> arraySlotArrangeInfo;
		GetDummyTestSlotArrangementInfo(arraySlotArrangeInfo);

		_mapSlotArrangInfo.Add(categoryId, arraySlotArrangeInfo);

		// 서버 선택 처리
		int serverPickSlotId = FMath::RandRange(0, 9);

		_mapCategoryServerPickSlotId.Add(categoryId, serverPickSlotId);
	}
}
// 테스트 배치 데이터 만들기(카테고리별)
void FGsStarLegacyManager::MakeTestSlotArrangementInfoByCategoryId(int In_categoryId)
{
	TArray<FGsStarLegacySlotArrangementInfo*> arraySlotArrangeInfo;
	GetDummyTestSlotArrangementInfo(arraySlotArrangeInfo);

	_mapSlotArrangInfo.Add(In_categoryId, arraySlotArrangeInfo);

	// 서버 선택 처리
	int serverPickSlotId = FMath::RandRange(0, 9);

	_mapCategoryServerPickSlotId.Add(In_categoryId , serverPickSlotId);
}
// 데이터 초기화
void FGsStarLegacyManager::InitializeData()
{	
	// 더미 데이터 만들지
	if (GetIsMakeTestStarLegacyData() == true)
	{
		// 기존데이터 클리어
		ClearAllSlotArrangeInfo();
		// 만듬
		MakeTestSlotArrangementInfo();
	}


	// 카테고리 처음걸로 세팅
	int firstCategoryId = GetFirstCategoryId();
	SetCurrentCategoryId(firstCategoryId);


	InitOpenInfo();
}

// 리롤 초기화
void FGsStarLegacyManager::InitializeReroll()
{
	// 더미 데이터 만들지
	if (GetIsMakeTestStarLegacyData() == true)
	{
		// 기존 카테고리 슬롯 정보 클리어
		ClearSlotArrangeInfoByCategory(_currentStarLegacyCategoryId);
		// 카테고리 슬롯 정보 만들기
		MakeTestSlotArrangementInfoByCategoryId(_currentStarLegacyCategoryId);
	}

	InitOpenInfo();
}
// 오픈 정보 초기화
void FGsStarLegacyManager::InitOpenInfo()
{
	// 선택 정보 초기화
	ClearSelectSlotId();

	// 오픈 갯수 초기화
	_currentOpenMagnitudeCount = 0;
}
// 카테고리 제일 앞에꺼 id
int FGsStarLegacyManager::GetFirstCategoryId()
{
	const UGsTable* catetoryTbl = FGsSchemaStarLegacyCategory::GetStaticTable();
	if (nullptr == catetoryTbl)
	{
		return -1;
	}

	TArray<const FGsSchemaStarLegacyCategory*> rowsCategoryData;
	catetoryTbl->GetAllRows<FGsSchemaStarLegacyCategory>(rowsCategoryData);
	int catogoryCount = rowsCategoryData.Num();
	if (catogoryCount == 0)
	{
		return -1;
	}

	if (rowsCategoryData[0] == nullptr)
	{
		return -1;
	}

	return rowsCategoryData[0]->id;
}

// 더미 정보 가져오기
void FGsStarLegacyManager::GetDummyTestSlotArrangementInfo(TArray<FGsStarLegacySlotArrangementInfo*>& Out_info)
{
	// 1. item id 랜덤으로 돌리기
	const UGsTable* itemCommonTbl = FGsSchemaItemCommon::GetStaticTable();
	if (nullptr == itemCommonTbl)
	{
		return;
	}

	TArray<const FGsSchemaItemCommon*> rowsItemCommonData;
	itemCommonTbl->GetAllRows<FGsSchemaItemCommon>(rowsItemCommonData);
	int itemCommonCount = rowsItemCommonData.Num();
	if (itemCommonCount == 0)
	{
		return;
	}

	// 10개 랜덤 데이터 만들기
	int slotOrder = 0;
	for (int i = 0; i < MAX_SLOT_COUNT; ++i)
	{
		int randomIndex = FMath::RandRange(0, itemCommonCount - 1);
		const FGsSchemaItemCommon* randItemCommonTbl = rowsItemCommonData[randomIndex];

		if (randItemCommonTbl == nullptr)
		{
			continue;
		}

		FGsStarLegacySlotArrangementInfo* slotArrangeInfo = new FGsStarLegacySlotArrangementInfo();

		// item id 저장
		slotArrangeInfo->_itemId = randItemCommonTbl->id;

		int itemCount = 1;
		int enchantCount = 0;

		// 장비면 강화, 나머지는 수량
		if (randItemCommonTbl->type == ItemType::EQUIP)
		{
			itemCount = 1;
			enchantCount = FMath::RandRange(0, 10);
		}
		else
		{
			itemCount = FMath::RandRange(1, 10);
			enchantCount = 1;
		}
		// 수량
		slotArrangeInfo->_itemCount = itemCount;
		// 강화 수치
		slotArrangeInfo->_enchantCount = enchantCount;

		// n등성
		slotArrangeInfo->_magnitude = FMath::RandRange(1, 5);
		// 슬롯 순서
		slotArrangeInfo->_slotOrderIndex = slotOrder;
		// 아이템 타입 저장
		slotArrangeInfo->_itemType = randItemCommonTbl->type;

		Out_info.Add(slotArrangeInfo);

		++slotOrder;
	}
}

// 별의 유산 game state 접근
FGsGameStateStarLegacy* FGsStarLegacyManager::GetGameStateStarLegacy()
{
	if (GMode() == nullptr)
	{
		return nullptr;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return nullptr;
	}
	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (contents == nullptr)
	{
		return nullptr;
	}

	FGsGameStateStarLegacy* stateStarLegacy =
		contents->GetContentsStateObject<FGsGameStateStarLegacy>(FGsContentsMode::InGame::ContentsStarLegacy);

	if (stateStarLegacy == nullptr)
	{
		return nullptr;
	}
	return stateStarLegacy;
}

// 별의 유산 config 테이블 접근
const FGsSchemaStarLegacyConfig* FGsStarLegacyManager::FindStarLegacyConfig()
{
	const UGsTable* starLegacyConfigDataTbl = FGsSchemaStarLegacyConfig::GetStaticTable();

	if (nullptr == starLegacyConfigDataTbl)
	{
		return nullptr;
	}

	TArray<const FGsSchemaStarLegacyConfig*> rowsStarLegacyConfigData;
	starLegacyConfigDataTbl->GetAllRows<FGsSchemaStarLegacyConfig>(rowsStarLegacyConfigData);

	if (rowsStarLegacyConfigData.Num() == 0)
	{
		return nullptr;
	}

	return rowsStarLegacyConfigData[0];
}
// 최대 등급 오픈 갯수
int FGsStarLegacyManager::GetMaxMagnitudeOpenCount(FairyGrade In_fairyGrade)
{
	const FGsSchemaStarLegacyConfig* starLegacyConfigTbl =  FindStarLegacyConfig();
	if (starLegacyConfigTbl == nullptr)
	{
		return -1;
	}

	for (auto iter: starLegacyConfigTbl->identifyChance)
	{
		if (In_fairyGrade ==  iter.FairyGrade)
		{			
			return iter.Count;
		}
	}
	// 없음
	return -1;
}
// 슬롯 정보 가져오기
TArray<FGsStarLegacySlotArrangementInfo*> FGsStarLegacyManager::GetSlotInfo(int In_catetoryId)
{
	return _mapSlotArrangInfo.FindRef(In_catetoryId);
}

// 선택 슬롯 클리어
void FGsStarLegacyManager::ClearSelectSlotId()
{
	if (_arraySelectSlotIds.Num() == 0)
	{
		return;
	}

	_arraySelectSlotIds.Empty();
}

// 선택된 id인가
bool FGsStarLegacyManager::IsSelectedId(int In_slotId)
{
	return _arraySelectSlotIds.Contains(In_slotId)? true: false;
}
// 선택 슬롯 id 추가
void FGsStarLegacyManager::AddSelectSlotId(int In_slotId)
{
	// 이미 있다면 할필요 없음
	if (_arraySelectSlotIds.Contains(In_slotId) == true)
	{
		return;
	}

	_arraySelectSlotIds.Add(In_slotId);
}
// 선택 슬롯 id 삭제
void FGsStarLegacyManager::DelSelectSlotId(int In_slotId)
{
	// 없으면 할필요 없음
	if (_arraySelectSlotIds.Contains(In_slotId) == false)
	{
		return;
	}
	_arraySelectSlotIds.Remove(In_slotId);
}
// 현재 탭(카테고리, n성계) 테이블 접근
const FGsSchemaStarLegacyCategory* FGsStarLegacyManager::GetCurrentCategoryTbl()
{
	const FGsSchemaStarLegacyCategory* findCategoryTbl = 
		UGsTableUtil::FindRowById<UGsTableStarLegacyCategory, FGsSchemaStarLegacyCategory>(_currentStarLegacyCategoryId);

	return findCategoryTbl;
}
// 선택 슬롯 갯수
int FGsStarLegacyManager::GetSelectSlotCount()
{
	return _arraySelectSlotIds.Num();
}

// 한칸 넘어갈때 대기 시간(틱 시간)
float FGsStarLegacyManager::GetTimeTickWaitingTimeToPass(int In_index)
{
	if (_arrayTimeTickWaitingTimeToPass.Num() <= In_index)
	{
		return 0.0f;
	}

	return _arrayTimeTickWaitingTimeToPass[In_index];
}

// 서버에서 pick 한 슬롯 id
int FGsStarLegacyManager::GetServerPickSlotId()
{
	 return _mapCategoryServerPickSlotId.FindRef(_currentStarLegacyCategoryId);
}

// 탭인덱스(0,1)로 카테고리 정보 가져오기
const FGsStarLegacyCategoryInfo* FGsStarLegacyManager::FindCategoryInfoByTabIndex(int In_tablIndex)
{
	if (_arrayCategoryIds.Num() <= In_tablIndex)
	{
		return nullptr;
	}

	int categoryId = _arrayCategoryIds[In_tablIndex];
	return _mapCategoryInfo.FindRef(categoryId);
}