#include "GsSpiritShotmanager.h"
#include "GsUnlockManager.h"
#include "Management/GsMessageHolder.h"
#include "../Message/MessageParam/GsMessageParam.h"


#include "Message/MessageParam/GsSpiritShotMessageParam.h"

#include "DataSchema/SpiritShot/GsSchemaSpiritShotLevel.h"
#include "DataSchema/SpiritShot/GsSchemaSpiritShotCategory.h"
#include "DataSchema/SpiritShot/GsSchemaSpiritShotEnchantSumLevel.h"


#include "SpiritShot/GsSpiritShotCategoryData.h"
#include "SpiritShot/GsSpiritShotLevelData.h"
#include "SpiritShot/GsSpiritShotStatData.h"

#include "Algo/Sort.h"

FGsSpiritShotManager::FGsSpiritShotManager() :
	_sumStatData(MakeShared<FGsSpiritShotStatData>())
{
}

void FGsSpiritShotManager::Initialize()
{
	if (!InitializeData())
	{
		GSLOG(Error, TEXT("!InitliazeData()"));
		return;
	}

	//RegisterMessages();
}

void FGsSpiritShotManager::Finalize()
{
	// 데이터 비우기
	_levelIdToCategoryId.Empty();
	_levelIdToData.Empty();
	_categoryIdToData.Empty();
	_categoryIdToLevelIdList.Empty();
	_categoryIdList.Empty();
	_consumeStatData.Empty();
	_statSumDataList.Empty();
	_unlockIdToCategoryId.Empty();
	_levelIdToPassivityId.Empty();

	//UnRegisterMessages();
}
//
//void FGsSpiritShotManager::RegisterMessages()
//{
//	FGsMessageHolder* messageHolder = GMessage();
//	if (messageHolder)
//	{
//		_unlockDelegate = messageHolder->GetUnlock().AddRaw(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK, this, &FGsSpiritShotManager::OnContentsUnlock);
//	}
//}
//
//void FGsSpiritShotManager::UnRegisterMessages()
//{
//	FGsMessageHolder* messageHolder = GMessage();
//	if (messageHolder)
//	{
//		messageHolder->GetUnlock().Remove(_unlockDelegate);
//	}
//}

bool FGsSpiritShotManager::InitializeData()
{
	// 테이블 데이터 가져오기
	const UGsTableSpiritShotLevel* levelTable = Cast<UGsTableSpiritShotLevel>(FGsSchemaSpiritShotLevel::GetStaticTable());
	if (nullptr == levelTable)
	{
		GSLOG(Error, TEXT("nullptr == levelTable"));
		return false;
	}

	TArray<const FGsSchemaSpiritShotLevel*> levelList;
	levelTable->GetAllRows(levelList);

	// 레벨 데이터 및 레벨ID 매핑 정보 캐싱
	for (const FGsSchemaSpiritShotLevel* level : levelList)
	{
		_levelIdToData.Emplace(level->id, MakeShared<FGsSpiritShotLevelData>(level));

		SpiritShotCategoryId categoryId = level->categoryId.GetRow()->id;
		_levelIdToCategoryId.Emplace(level->id, categoryId);

		if (!_categoryIdToLevelIdList.Contains(categoryId))
		{
			_categoryIdToLevelIdList.Emplace(categoryId);
		}
		_categoryIdToLevelIdList[categoryId].Emplace(level->id);

		// PassivitySetId 연결 상태 확인 (연결 안되었으면 0 을 삽입한다)
		if (const FGsSchemaPassivitySet* passivitySet = level->passivityId.GetRow())
		{
			_levelIdToPassivityId.Emplace(level->id, passivitySet->id);
		}
		else
		{
			_levelIdToPassivityId.Emplace(level->id, INVALID_PASSIVITY_ID);
		}
	}

	// 카테고리별 레벨 리스트는 카테고리 Id 값 기준으로 오름차순 정렬
	_categoryIdToLevelIdList.KeySort([](const SpiritShotCategoryId& A, const SpiritShotCategoryId& B)
		{
			return A < B;
		});

	// 카테고리에 붙은 레벨들도 Id 값 기준 오름차순 정렬
	for (auto& categotyIdLevelListPair : _categoryIdToLevelIdList)
	{
		Algo::Sort(categotyIdLevelListPair.Value, [](const SpiritShotLevelId& A, const SpiritShotLevelId& B)
			{
				return A < B;
			});
	}

	// 각 단계의 레벨 데이터에 대해, 다음 레벨에 해당하는 레벨의 Id 를 세팅
	{
		SpiritShotLevelId prevId;
		SpiritShotLevelId curId;

		// 카테고리 순회
		for (const auto& pair : _categoryIdToLevelIdList)
		{
			// 초깃값 세팅 (for loop 은
			if (!pair.Value.IsValidIndex(0))
			{
				GSLOG(Error, TEXT("!pair.Value.IsValidIndex(0)"));
				continue;
			}
			prevId = pair.Value[0];

			// 레벨 리스트 순회
			for (int32 i = 1; i < pair.Value.Num(); ++i)
			{
				curId = pair.Value[i];
				_levelIdToData[prevId]->SetNextLevelId(curId);
				prevId = curId;
			}
		}
	}

	// 각 단계의 레벨 데이터에 대해, 누적 StatData 세팅
	{
		// 카테고리 순회
		for (const auto& pair : _categoryIdToLevelIdList)
		{
			// 레벨별 누적 PassivityIdList. 카테고리별로 나뉠 것이다
			TArray<PassivityId> passivityIdList;

			// LevelIdList 비어있는지 점검
			if (pair.Value.Num() == 0)
			{
				GSLOG(Error, TEXT("!pair.Value.IsValidIndex(0)"));
				continue;
			}

			// 레벨 리스트 순회하며 StatData 생성 후, _levelIdToData 에 매칭
			for (SpiritShotLevelId levelId : pair.Value)
			{
				passivityIdList.Emplace(_levelIdToPassivityId[levelId]);
				_levelIdToData[levelId]->SetStatData(passivityIdList);

			}
		}
	}


	// 카테고리 데이터 매핑 시작
	const UGsTableSpiritShotCategory* categoryTable = Cast<UGsTableSpiritShotCategory>(FGsSchemaSpiritShotCategory::GetStaticTable());
	if (nullptr == categoryTable)
	{
		GSLOG(Error, TEXT("nullptr == categoryTable"));
		return false;
	}
	
	TArray<const FGsSchemaSpiritShotCategory*> categoryList;
	categoryTable->GetAllRows(categoryList);

	// 카테고리 관련 컨테이너 초기화
	for (const FGsSchemaSpiritShotCategory* categoryData : categoryList)
	{
		SpiritShotCategoryId categoryId = categoryData->id;

		// 카테고리가 없거나 카테고리에 레벨이 전혀 존재하지 않음
		if (!_categoryIdToLevelIdList.Contains(categoryId) || !_categoryIdToLevelIdList[categoryId].IsValidIndex(0))
		{
			GSLOG(Error, TEXT("No Level Data for Category Id : %d"), categoryId);
			continue;
		}

		// 카테고리의 초기 레벨 값
		SpiritShotLevelId initialLevel = _categoryIdToLevelIdList[categoryId][0];

		// 각 카테고리의 레벨 초기값은 초기 레벨 값으로 설정
		_categoryIdToData.Emplace(categoryId, MakeShared<FGsSpiritShotCategoryData>(categoryData, *_levelIdToData.Find(initialLevel)));

		// 초기 레벨의 다음 레벨 값
		SpiritShotLevelId nextLevelId = _levelIdToData[initialLevel]->GetNextLevelId();

		// 카테고리 Id 정렬할 리스트 매핑
		_categoryIdList.Emplace(categoryId);

		// 언락 Id 와 카테고리 Id 매핑
		_unlockIdToCategoryId.Emplace(categoryId, categoryData->unlockContentsId.GetRow()->id);
	}

	// 카테고리 - 데이터 맵 정렬 (CategoryId 기준)
	_categoryIdToData.KeySort([](const SpiritShotCategoryId& A, const SpiritShotCategoryId& B)
		{
			return A < B;
		});

	// 카테고리 Id 리스트 정렬
	Algo::Sort(_categoryIdList, [](const SpiritShotCategoryId& A, const SpiritShotCategoryId& B) 
		{
			return A < B;
		});


	// 천마석 누적 합산 레벨 데이터 파싱
	const UGsTableSpiritShotEnchantSumLevel* enchantSumTable = Cast<UGsTableSpiritShotEnchantSumLevel>(FGsSchemaSpiritShotEnchantSumLevel::GetStaticTable());
	if (nullptr == enchantSumTable)
	{
		GSLOG(Error, TEXT("nullptr == enchantSumTable"));
		return false;
	}

	TArray<const FGsSchemaSpiritShotEnchantSumLevel*> enchantSumLevelList;
	enchantSumTable->GetAllRows(enchantSumLevelList);


	for (const FGsSchemaSpiritShotEnchantSumLevel* enchantSumLevel : enchantSumLevelList)
	{
		if (enchantSumLevel->passivityId.GetRow())
		{
			_sumLevelPassivityIdPairList.Emplace(enchantSumLevel->needLevelSum, enchantSumLevel->passivityId.GetRow()->id);
		}
		else
		{
			// 유효한 패시비티 Id 가 있는 경우에만 캐싱을 진행함
			//_sumLevelPassivityIdPairList.Emplace(enchantSumLevel->needLevelSum, INVALID_PASSIVITY_ID);
		}

		_sumLevelEffectDataPairList.Emplace(enchantSumLevel->needLevelSum, enchantSumLevel->spiritShotEffect);
	}

	// 기타 변수값들 초기화
	_sumLevel = 0;

	return true;
}

void FGsSpiritShotManager::SpiritShotInfo(const IGsMessageParam* InParam)
{
	// 레벨 리스트 읽기
	const FGsSpiritShotMessageParamInfo* messageParamInfo = static_cast<const FGsSpiritShotMessageParamInfo*>(InParam);
	if (nullptr == messageParamInfo)
	{
		GSLOG(Error, TEXT("nullptr == messageParamInfo"));
		return;
	}

	// 레벨 Id 별 알맞은 카테고리 관련 정보 매칭
	for (const SpiritShotLevelId& levelId : messageParamInfo->_spiritShotLevelIdList)
	{
		// 레벨 데이터에 카테고리 Id 누락
		if (!_levelIdToCategoryId.Contains(levelId))
		{
			GSLOG(Error, TEXT("No categoryId for Level Id : %d"), levelId);
			continue;
		}

		SpiritShotCategoryId categoryId = *_levelIdToCategoryId.Find(levelId);

		// 레벨 Id 에 맞는 데이터 생성이 되지 않음
		if (!_levelIdToData.Contains(levelId))
		{
			GSLOG(Error, TEXT("No Level Data for Level Id : %d"), levelId);
			continue;
		}

		// 카테고리 데이터 컨테이너 초기화가 제대로 이루어지지 않음
		if (!_categoryIdToData.Contains(categoryId))
		{
			GSLOG(Error, TEXT("No Category Data for Category Id : %d"), categoryId);
			continue;
		}

		// 카테고리 Id 와 알맞은 레벨 데이터 매칭
		_categoryIdToData[categoryId]->SetLevelData(*_levelIdToData.Find(levelId));
	}

	// 카테고리 데이터 별로, 언락 상태 설정

	for (const auto& pair : _categoryIdToData)
	{
		SpiritShotCategoryId categoryId = pair.Key;
		SpiritShotLevelId levelId = pair.Value->GetLevelData()->GetLevelId();

		// 현재 상태 매칭
		EGsSpiritShotEnchantState state = EGsSpiritShotEnchantState::UnKnown;

		FGsUnlockManager* unlockManager = GSUnlock();
		if (nullptr == unlockManager)
		{
			GSLOG(Error, TEXT("nullptr == unlockManager"));
			return;
		}

		// 언락 여부 체크 (언락 매니저는 인게임 진입시가 아닌, 스폰 시에 데이터를 갱신하므로 실제로는 다 잠겨버린다)
		if (!unlockManager->IsContentsUnlock(_categoryIdToData[categoryId]->GetUnlockContentsId()))
		{
			state = EGsSpiritShotEnchantState::Locked;
		}
		else
		{
			// 마지막 레벨까지 올렸을 경우, Completed 로 설정
			state = (_categoryIdToLevelIdList[categoryId].Last() == levelId) ? EGsSpiritShotEnchantState::Completed : EGsSpiritShotEnchantState::Activated;
		}

		_categoryIdToData[categoryId]->SetCurrentState(state);
	}

	// 누적 관련 데이터 업데이트
	InvalidateSumData();
}

void FGsSpiritShotManager::AckEnchant(const IGsMessageParam* InParam)
{
	// 카테고리 및 레벨 읽기
	const FGsSpiritShotMessageParamAckEnchant* ackEnchantParam = static_cast<const FGsSpiritShotMessageParamAckEnchant*>(InParam);
	if (nullptr == ackEnchantParam)
	{
		GSLOG(Error, TEXT("nullptr == ackEnchantParam"));
		return;
	}

	SpiritShotCategoryId categoryId = ackEnchantParam->_spiritShotCategoryId;
	SpiritShotLevelId levelId = ackEnchantParam->_spiritShotLevelId;

	// 서버에서 이상한 레벨 값을 주었을 경우, 레벨 데이터 변경하지 않음
	if (!_levelIdToData.Contains(levelId))
	{
		GSLOG(Error, TEXT("!_levelIdToData.Contains(levelId), Level Id : %d"), levelId);
		return;
	}

	// 해당 카테고리의 레벨 정보 업데이트
	_categoryIdToData[categoryId]->SetLevelData(_levelIdToData[levelId]);

	// 만약 최대 레벨에 도달했을 경우, 상태를 Completed 로 설정 (UI 에서 해당 상태 조회하여 강화 버튼 막을 것임)
	if (_categoryIdToLevelIdList[categoryId].Last() == levelId)
	{
		_categoryIdToData[categoryId]->SetCurrentState(EGsSpiritShotEnchantState::Completed);
	}

	// 총합 레벨 및 누적 스탯 데이터 업데이트
	InvalidateSumData();
}

bool FGsSpiritShotManager::OnUnlockCategory(UnlockContentsId InContentsId)
{
	if (!_unlockIdToCategoryId.Contains(InContentsId))
	{
		GSLOG(Error, TEXT("!_unlockIdToCategoryId.Contains(InContentsId)"));
		return false;
	}

	SpiritShotCategoryId categoryId = _unlockIdToCategoryId[InContentsId];

	bool isChanged = false;

	// 만약 잠겨 있었다면, 활성/완료 상태로 바꾸어 준다
	if (_categoryIdToData[categoryId]->GetCurrentState() == EGsSpiritShotEnchantState::Locked)
	{
		// 마지막 레벨까지 올렸을 경우, Completed 로 설정
		EGsSpiritShotEnchantState state = (_categoryIdToData[categoryId]->GetLevelData()->GetNextLevelId() == INVALID_SPIRIT_SHOT_LEVEL_ID) ? EGsSpiritShotEnchantState::Completed : EGsSpiritShotEnchantState::Activated;

		_categoryIdToData[categoryId]->SetCurrentState(state);

		isChanged = true;
	}

	if (isChanged)
	{
		InvalidateSumData();
	}

	return isChanged;
}

int32 FGsSpiritShotManager::GetSumLevel() const
{
	return _sumLevel;
}

void FGsSpiritShotManager::InvalidateSumData()
{
	// 합산 레벨부터 갱신
	_sumLevel = 0;

	TArray<PassivityId> _sumPassivityIdList;

	for (const auto& categoryDataPair : _categoryIdToData)
	{
		// 잠식 합산 효과는 계정별 공유로, 잠겨 있는 것 여부와 관계없이 적용되어야 함
		// 잠겨 있을 경우, 합산 레벨에 반영하지 않음 (UnKnown 상태인 것은 로직상 불가능함)
		_sumLevel += categoryDataPair.Value->GetLevelData()->GetLevel();

		TSharedRef<FGsSpiritShotStatData> statData = categoryDataPair.Value->GetLevelData()->GetStatData();
		_sumPassivityIdList.Append(statData->GetPassivityIdList());
	}


	// 합산 레벨에 맞는 스탯 데이터 생성
	_sumStatData->SetStatData(MoveTemp(_sumPassivityIdList));

	// 합산 레벨에 맞게 컬렉션 데이터 업데이트
	InvalidatePassivityCollectionData();
	InvalidateSumPassivityCollectionData();
}

void FGsSpiritShotManager::InvalidatePassivityCollectionData()
{
	_passivityCollectionData.Empty();

	_passivityCollectionData.Append(GetTotalPassivityIdList());

	// 컬렉션 데이터 갱신 요청
	if (_passivityCollectionData.GetPassivityCount() > 0)
	{
		FGsPrimitiveInt32 param(static_cast<int32>(PassivityOwnerType::SPIRIT_SHOT_ENCHANT));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &param);
	}
}

void FGsSpiritShotManager::InvalidateSumPassivityCollectionData()
{
	_sumPassivityCollectionData.Empty();

	for (const auto& pair : GetSumLevelPassivityIdPairList())
	{
		if (pair.Key > _sumLevel)
		{
			break;
		}
		_sumPassivityCollectionData.Add(pair.Value);
	}

	// 컬렉션 데이터 갱신 요청
	if (_sumPassivityCollectionData.GetPassivityCount() > 0)
	{
		FGsPrimitiveInt32 param(static_cast<int32>(PassivityOwnerType::SPIRIT_SHOT_ENCHANT_SUM_LEVEL));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &param);
	}
}

//void FGsSpiritShotManager::OnContentsUnlock(const struct IGsMessageParam* InParam)
//{
//	const FGsPrimitivePairInt32* param = StaticCast<const FGsPrimitivePairInt32*>(InParam);
//	if (nullptr == param)
//	{
//		GSLOG(Error, TEXT("nullptr == param"));
//		return;
//	}
//	
//	UnlockCategory(param->_second);
//}

SpiritShotCategoryId FGsSpiritShotManager::GetDefaultCategoryId() const
{
	if (!_categoryIdList.IsValidIndex(0))
	{
		GSLOG(Error, TEXT("!_categoryIdList.IsValidIndex(0)"));
		return INVALID_SPIRIT_SHOT_CATEGORY_ID;
	}
		
	return _categoryIdList[0];
}

SpiritShotLevelId FGsSpiritShotManager::GetDefaultLevelId() const
{
	SpiritShotCategoryId defaultCategoryId = GetDefaultCategoryId();

	if (!_categoryIdToLevelIdList.Contains(defaultCategoryId))
	{
		GSLOG(Error, TEXT("!_categoryIdToLevelIdList.Contains(defaultCategoryId)"));
		return INVALID_SPIRIT_SHOT_LEVEL_ID;
	}

	if (!_categoryIdToLevelIdList[defaultCategoryId].IsValidIndex(0))
	{
		GSLOG(Error, TEXT("!_categoryIdToLevelIdList[defaultCategoryId].IsValidIndex(0)"));
		return INVALID_SPIRIT_SHOT_LEVEL_ID;
	}

	return _categoryIdToLevelIdList[defaultCategoryId][0];
}

const FGsSpiritShotCategoryData* FGsSpiritShotManager::GetCategoryData(const SpiritShotCategoryId InCategoryId) const
{
	// 만약 InCategoryId 가 0 (기본값) 이면, 첫 항목을 리턴
	// 여기에 도달한다는 것은, Window 히스토리 부분에서 문제가 생겼음을 의미한다
	//if (0 == InCategoryId)
	//{
	//	return &((*_categoryIdToData.begin()).Value.Get());
	//}

	if (!_categoryIdToData.Contains(InCategoryId))
	{
		GSLOG(Error, TEXT("!_categoryIdToData.Contains(InCategoryId)"));
		return nullptr;
	}

	return &(_categoryIdToData[InCategoryId].Get());
}

const FGsSpiritShotLevelData* FGsSpiritShotManager::GetLevelData(const SpiritShotCategoryId InCategoryId) const
{
	const FGsSpiritShotCategoryData* categoryData = GetCategoryData(InCategoryId);

	if (nullptr == categoryData)
	{
		GSLOG(Error, TEXT("nullptr == categoryData"));
		return nullptr;
	}

	return &(categoryData->GetLevelData().Get());
}

const FGsSpiritShotLevelData* FGsSpiritShotManager::GetLevelDataWithLevel(const SpiritShotLevelId InLevelId) const
{
	if (!_levelIdToData.Contains(InLevelId))
	{
		GSLOG(Log, TEXT("!_levelIdToData.Contains(InLevelId), InLevelId : %d"), InLevelId);
		return nullptr;
	}

	return &(_levelIdToData[InLevelId].Get());
}

const FGsSpiritShotCategoryData* FGsSpiritShotManager::GetCategoryDataAtIndex(const int32 InIndex) const
{
	if (!_categoryIdList.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_categoryIdList.IsValidIndex(InIndex), InIndex : %d"), InIndex);
		return nullptr;
	}

	return GetCategoryData(_categoryIdList[InIndex]);
}

SpiritShotCategoryId FGsSpiritShotManager::GetCategoryIdAtIndex(const int32 InIndex) const
{
	if (!_categoryIdList.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_categoryIdList.IsValidIndex(InIndex), InIndex : %d"), InIndex);
		return INVALID_SPIRIT_SHOT_CATEGORY_ID;
	}

	return _categoryIdList[InIndex];
}

const TArray<SpiritShotCategoryId>& FGsSpiritShotManager::GetCategoryIdList() const
{
	return _categoryIdList;
}

int32 FGsSpiritShotManager::GetTotalCategoryCount() const
{
	return _categoryIdList.Num();
}

TSharedRef<FGsSpiritShotStatData> FGsSpiritShotManager::GetSumStatData() const
{
	return _sumStatData;
}

TArray<PassivityId> FGsSpiritShotManager::GetTotalPassivityIdList() const
{
	TArray<PassivityId> totalPassivityIdList;

	// 모든 카테고리의 ID 에 대해, 현재 플레이어가 도달한 카테고리별 레벨의 스탯 데이터 순회하며, PassivityId List 생성
	for (const SpiritShotCategoryId categoryId : _categoryIdList)
	{
		if (!_categoryIdToData.Contains(categoryId))
		{
			GSLOG(Error, TEXT("!_categoryIdToData.Contains(categoryId)"));
			continue;
		}
		
		TSharedRef<FGsSpiritShotStatData> statData = _categoryIdToData[categoryId]->GetLevelData()->GetStatData();
		totalPassivityIdList.Append(statData->GetPassivityIdList());
	}

	return MoveTemp(totalPassivityIdList);
}

TArray<TPair<Level, PassivityId>> FGsSpiritShotManager::GetSumLevelPassivityIdPairList() const
{
	return _sumLevelPassivityIdPairList;
}

const FGsSpiritShotPassivityCollection*  FGsSpiritShotManager::GetPassivityCollectionData() const
{
	return &_passivityCollectionData;
}

const FGsSpiritShotSumPassivityCollection* FGsSpiritShotManager::GetSumPassivityCollectionData() const
{
	return &_sumPassivityCollectionData;
}

FSoftObjectPath FGsSpiritShotManager::GetSpiritShotEffectPath(Level InLevel) const
{
	if (_sumLevelEffectDataPairList.Num() == 0)
	{
		return FSoftObjectPath();
	}

	for (const auto& pair : _sumLevelEffectDataPairList)
	{
		if (pair.Key >= InLevel)
		{
			return pair.Value.effectPath;
		}
	}

	return _sumLevelEffectDataPairList.Last().Value.effectPath;
}

float FGsSpiritShotManager::GetSpiritShotEffectScale(Level InLevel) const
{
	float scale = 0.0f;

	// scale 은 데이터 테이블에 백분율 값으로 들어가 있다
	for (const auto& pair : _sumLevelEffectDataPairList)
	{
		if (pair.Key >= InLevel)
		{
			scale = (float) pair.Value.effectScale;
			scale = FMath::Max(scale / 100.0f, 1.0f);
			return scale;
		}
	}
	
	scale = (float) _sumLevelEffectDataPairList.Last().Value.effectScale;
	scale = FMath::Max(scale / 100.0f, 1.0f);

	// scale 의 최솟값은 1.0 일 것이다 (아마도)
	return scale;
}