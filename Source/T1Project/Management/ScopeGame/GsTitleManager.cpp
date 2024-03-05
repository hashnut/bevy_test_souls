

#include "GsTitleManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "GsTableManager.h"
#include "DataSchema/Title/GsSchemaTitle.h"
#include "DataSchema/Title/GsSchemaTitleSubCategory.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "Title/GsTitleHelper.h"
#include "Title/GsTitlePlayerData.h"

#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Net/GsNetSendServiceWorld.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"

#include "T1Project/T1Project.h"


void FGsTitleManager::Initialize()
{
	isSkipMessage = true;

	_categoryDataHolder.Empty();
	_titleDataHolder.Empty();

	_titleDatasByCategoryId.Empty();

	_equipData.Clear();

	_collectionRewardData.BuildCollectionReward();

	BuildCategoryData();

	BuildTitle();

	isSkipMessage = false;

	_viewList.Empty();

	RegisterMessages();
}


void FGsTitleManager::Finalize()
{
	_categoryDataHolder.Empty();
	_titleDataHolder.Empty();

	_titleDatasByCategoryId.Empty();

	_equipData.Clear();

	_collectionRewardData.Finalize();

	_titleStatAll.Empty();

	isSkipMessage = false;

	_viewList.Empty();

	UnegisterMessages();
}


void FGsTitleManager::RegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	_listGameObjectDelegate.Emplace(MessageManager->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE
		, this
		, &FGsTitleManager::OnLocalPlayerSpawnComplete));
}


void FGsTitleManager::UnegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	for (const MsgGameObjHandle& ItemDelegate : _listGameObjectDelegate)
	{
		MessageManager->GetGameObject().Remove(ItemDelegate);
	}
	_listGameObjectDelegate.Empty();
}


void FGsTitleManager::SendToWindowMessage(MessageContentTitle InType, const struct IGsMessageParam* InMessage)
{
	CheckRedDot();

	if (true == isSkipMessage)
	{
		return;
	}
	
	GMessage()->GetTitle().SendMessage(InType, InMessage);
}


bool FGsTitleManager::CheckRedDot(bool bInUpdate /* = true */)
{
	bool isRedDot = false;

	do 
	{
		if (true == _collectionRewardData.IsRedDot())
		{
			isRedDot = true;
			break;
		}

		for (const auto& pair : _reddotCollector)
		{
			if (0 < pair.Value.Num())
			{
				isRedDot = true;
				break;
			}
		}
		
	} while (0);

	bool isPrevRedDot = IsRedDot();

	SetRedDot(isRedDot);

	if (isPrevRedDot != IsRedDot())
	{
		SetRedDot(isRedDot);
		// 2021/08/13 PKT - RedDot의 변경을 HUD에 알린다.
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::TITLE, false);
	}

	return IsRedDot();
}

void FGsTitleManager::TitleConfirm(TitleEquipSlot InSlot, int32 InIndex)
{
	if (false == _categoryDataHolder.Contains(InSlot))
	{
		GSLOG(Error, TEXT("false == _categoryDataHolder.Contains(InSlot)"));
		return;
	}

	if (false == _categoryDataHolder[InSlot].IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == _categoryDataHolder[InSlot].IsValidIndex(InIndex)"));
		return;
	}

	TitleCategoryId categoryId = _categoryDataHolder[InSlot][InIndex]->id;
	if (false == _titleDatasByCategoryId.Contains(categoryId))
	{
		GSLOG(Error, TEXT("false == _titleDatasByCategoryId.Contains(categoryId)"));
		return;
	}

	isSkipMessage = true;

	TArray<TitleId> confirmList;

	for (FGsTitleData* title : _titleDatasByCategoryId[categoryId])
	{
		if (title && title->IsNewAcquire())
		{
			title->Confirm();
			confirmList.Emplace(title->GetId());
		}
	}

	isSkipMessage = false;

	// 2022/01/24 PKT - Send Window message
	if (0 < confirmList.Num())
	{	// 2022/01/26 PKT - 서버에 확인 했음을 알림.
		FGsNetSendServiceWorld::Send_TitleConfirm(confirmList);

		// 2022/01/26 PKT - Window RedDot Update
		SendToWindowMessage(MessageContentTitle::UPDATE_CATEGORY_REDDOT, nullptr);
	}
}


void FGsTitleManager::CollectionConfirm()
{
	_collectionRewardData.Confirm();
}

void FGsTitleManager::BuildCategoryData()
{
	// 2022/01/13 PKT - Table Manager
	const UGsTableManager& tableManager = UGsTableManager::GetInstance();

	// 2022/01/13 PKT - Title Category
	const UGsTableTitleSubCategory* categoryTable
		= Cast<UGsTableTitleSubCategory>(tableManager.GetTable(TBaseStructure<FGsSchemaTitleSubCategory>::Get()));
	if (nullptr == categoryTable)
	{
		GSLOG(Error, TEXT("nullptr == categoryTable"));
		return;
	}

	const TMap<FName, uint8*>* RowMap = nullptr;

	if (false == categoryTable->GetRowMap(RowMap))
	{
		GSLOG(Error, TEXT("false == categoryTable->GetRowMap(RowMap)"));
		return;
	}

	// 2022/01/13 PKT - Add List
	for (auto& RowKvIt : *RowMap)
	{
		const FGsSchemaTitleSubCategory* data = reinterpret_cast<const FGsSchemaTitleSubCategory*>(RowKvIt.Value);
		if (nullptr == data)
		{
			GSLOG(Error, TEXT("nullptr == data"));
			return;
		}

		_categoryDataHolder.FindOrAdd(data->equipSlotType).AddUnique(data);
	}

	// 2022/01/18 PKT - Sort
	for (auto& pair : _categoryDataHolder)
	{
		pair.Value.Sort([](const FGsSchemaTitleSubCategory& lhr, const FGsSchemaTitleSubCategory& rhr)
			{
				return lhr.displayOrderNo < rhr.displayOrderNo;
			}
		);
	}
}


void FGsTitleManager::BuildTitle()
{
	// 2022/01/13 PKT - Table Manager
	const UGsTableManager& tableManager = UGsTableManager::GetInstance();

	// 2022/01/13 PKT - Title
	const UGsTableTitle* titleTable = Cast<UGsTableTitle>(tableManager.GetTable(TBaseStructure<FGsSchemaTitle>::Get()));
	if (nullptr == titleTable)
	{
		GSLOG(Error, TEXT("nullptr == titleTable"));
		return;
	}

	const TMap<FName, uint8*>* RowMap = nullptr;

	if (false == titleTable->GetRowMap(RowMap))
	{
		GSLOG(Error, TEXT("false == titleTable->GetRowMap(RowMap)"));
		return;
	}

	_titleDataHolder.Reserve(RowMap->Num());

	// 2022/01/13 PKT - Add List
	for (auto& RowKvIt : *RowMap)
	{
		const FGsSchemaTitle* data = reinterpret_cast<const FGsSchemaTitle*>(RowKvIt.Value);
		if (nullptr == data)
		{
			GSLOG(Error, TEXT("nullptr == data"));
			return;
		}

		auto& titleData = _titleDataHolder.Emplace_GetRef(FGsTitleData(data));

		titleData.OnStateChangedDelegate().BindRaw(this, &FGsTitleManager::OnStateChangedTitleData);

		// 2022/01/17 PKT - Stat Collector
		const int32 maxCount = titleData.GetStatCount();
		for (int32 idx = 0; idx < maxCount; ++idx)
		{	// 2022/01/17 PKT - 중복되지 않게 넣어 준다.
			_titleStatAll.AddUnique(titleData.GetStatType(idx));
		}

		TitleEquipSlot equipSlot = titleData.GetEquipSlotType();
		TitleCategoryId categoryId = titleData.GetCategoryId();

		if (false == _categoryDataHolder.Contains(equipSlot))
		{	// 2022/01/20 PKT - Title 있는 카테고리 정보로 카테고리를 찾을 수 없음.
			GSLOG(Error, TEXT("not find Category : Title ID[%d], CategoaryId[%d]"), titleData.GetId(), categoryId);
			continue;
		}

		if (nullptr == _categoryDataHolder[equipSlot].FindByPredicate([categoryId](const FGsSchemaTitleSubCategory* InData)
			{
				return (InData->id == categoryId);
			}
		))
		{	// 2022/01/20 PKT - Title 있는 카테고리 정보로 카테고리를 찾을 수 없음.
			GSLOG(Error, TEXT("not find Category : Title ID[%d], CategoaryId[%d]"), titleData.GetId(), categoryId);
			continue;
		}

		// 2022/09/01 PKT - 전체 카테고리에 전부 넣어 준다.
		if (_categoryDataHolder[equipSlot].IsValidIndex(ALL_CATEGORY_INDEX))
		{
			const FGsSchemaTitleSubCategory* schemaCategoryData = _categoryDataHolder[equipSlot][ALL_CATEGORY_INDEX];
			if (schemaCategoryData && INVALID_TITLE_CATEGORY_ID != schemaCategoryData->id )
			{
				_titleDatasByCategoryId.FindOrAdd(schemaCategoryData->id).Emplace(&titleData);
			}
		}
	}

	for (int32 idx = 0; idx < StaticCast<int32>(TitleEquipSlot::MAX); ++idx)
	{
		const FGsSchemaTitleSubCategory* schemaSubCategory = _categoryDataHolder[StaticCast<TitleEquipSlot>(idx)][ALL_CATEGORY_INDEX];
		TitleCategoryId allCategoryId = schemaSubCategory->id;

		if (false == _titleDatasByCategoryId.Contains(allCategoryId))
		{
			GSLOG(Error, TEXT("false == _titleDatasByCategoryId.Contains(allCategoryId) %d"), allCategoryId);
			continue;
		}

		// 2022/09/01 PKT - 전체 카테고리 만 정렬
		_titleDatasByCategoryId[allCategoryId].Sort([](const FGsTitleData& lhr, const FGsTitleData& rhr)
			{
				if (lhr.GetGrade() > rhr.GetGrade())
				{
					return true;
				}
				else if (lhr.GetGrade() < rhr.GetGrade())
				{
					return false;
				}

				return lhr.GetId() < rhr.GetId();
			}
		);

		const TArray<FGsTitleData*> allCategoryTitleSet = _titleDatasByCategoryId[allCategoryId];
		for (FGsTitleData* titleData : allCategoryTitleSet)
		{
			TitleCategoryId findCategoryId = titleData->GetCategoryId();
			if (findCategoryId == allCategoryId)
			{
				GSLOG(Error, TEXT("findCategoryId == allCategoryId"));
				return;
			}

			_titleDatasByCategoryId.FindOrAdd(findCategoryId).Emplace(titleData);
		}
	}

	_titleStatAll.Sort([](const StatType& lhr, const StatType& rhr)
		{
			return lhr < rhr;
		}
	);
}

void FGsTitleManager::UpdateBillboardTitle()
{
	/**
	 * HUD Update
	 */
	UGsGameObjectBase* object = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	UGsGameObjectLocalPlayer* localPlayer = (nullptr != object) ? Cast<UGsGameObjectLocalPlayer>(object) : nullptr;
	FGsGameObjectDataLocalPlayer* localData = (nullptr != localPlayer) ? (localPlayer->GetCastData<FGsGameObjectDataLocalPlayer>()) : nullptr;
	if (nullptr == localData)
	{
		return;
	}

	FGsTitlePlayerData::TitleSet frontTitleSet = FGsTitlePlayerData::InitData;
	FGsTitlePlayerData::TitleSet backTitleSet = FGsTitlePlayerData::InitData;

	if (_equipData.GetData(TitleEquipSlot::FRONT))
	{
		frontTitleSet = FGsTitlePlayerData::TitleSet(
			_equipData.GetData(TitleEquipSlot::FRONT)->GetTitle(), _equipData.GetData(TitleEquipSlot::FRONT)->GetTitleHUDColor());
	}
	
	if (_equipData.GetData(TitleEquipSlot::BACK))
	{
		backTitleSet = FGsTitlePlayerData::TitleSet(
			_equipData.GetData(TitleEquipSlot::BACK)->GetTitle(), _equipData.GetData(TitleEquipSlot::BACK)->GetTitleHUDColor());
	}
	localData->SetTitleData(frontTitleSet, backTitleSet);

	// 2022/02/03 PKT - Billboard Update
	localPlayer->UpdateBillboard(EGsBillboardUpdateType::Title);
}

void FGsTitleManager::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	UpdateBillboardTitle();
}


void FGsTitleManager::OnStateChangedTitleData(const FGsTitleData* InData, ETitleMessageReason InMessage)
{
	// 2022/01/24 PKT - RedDot 정보 수집
	if (InData->IsNewAcquire())
	{
		_reddotCollector.FindOrAdd(InData->GetCategoryId()).AddUnique(InData->GetId());
	}
	else
	{
		if (true == _reddotCollector.Contains(InData->GetCategoryId()))
		{
			_reddotCollector[InData->GetCategoryId()].Remove(InData->GetId());
		}		
	}
	

	// 2022/01/14 PKT - Send Message
	const FGsPrimitiveInt32 message(StaticCast<int32>(InData->GetId()));	
	SendToWindowMessage(MessageContentTitle::UPDATE_TITLE, &message);

	if (ETitleMessageReason::eEquipState == InMessage)
	{	// 2022/01/14 PKT - 장착 정보도 갱신 한다.
		UpdateBillboardTitle();
		// 2022/02/03 PKT - windw message
		SendToWindowMessage(MessageContentTitle::UPDATE_EQUIP, &message);
	}
}

// 2022/01/21 PKT - 칭호 관련 초기 세팅값
void FGsTitleManager::InitTitleData(const TArray<TitleInfo>& InTitleList, const TPair<TitleId, TitleId>& InEquipIds)
{
	isSkipMessage = true;

	int32 collectionCount = 0;

	// 2022/01/21 PKT - 칭호 상태 설정
	for (const TitleInfo& title : InTitleList)
	{
		FGsTitleData* titleData = FindTitleDataInHolder(title.mTitleId);
		if (nullptr == titleData)
		{
			GSLOG(Error, TEXT("nullptr == titleData : [%d]"), title.mTitleId);
			continue;
		}

		titleData->UpdateStatus(title.mStatus, title.mConditionCount);

		if ( titleData->IsAcquire() )
		{
			++collectionCount;
		}
	}

	_collectionRewardData.Initialized(collectionCount, _titleDataHolder.Num());

	// 2022/01/21 PKT - 장착 상태 설정
	_equipData.Clear();

	if (INVALID_TITLE_ID != InEquipIds.Key)
	{
		FGsTitleData* titleData = FindTitleDataInHolder(InEquipIds.Key);
		if (nullptr == titleData)
		{
			GSLOG(Error, TEXT("nullptr == front titleData : [%d]"), InEquipIds.Key);
		}
		else
		{
			_equipData.Equip(titleData, false);
		}
	}

	if (INVALID_TITLE_ID != InEquipIds.Value)
	{
		FGsTitleData* titleData = FindTitleDataInHolder(InEquipIds.Value);
		if (nullptr == titleData)
		{
			GSLOG(Error, TEXT("nullptr == back titleData : [%d]"), InEquipIds.Value);
		}
		else
		{
			_equipData.Equip(titleData, false);
		}
	}

	isSkipMessage = false;

	CheckRedDot();
}

// 2022/01/21 PKT - 칭호 상태(진행도) 변경
void FGsTitleManager::UpdateTitle(const TitleInfo& InTitleInfo)
{
	FGsTitleData* titleData = FindTitleDataInHolder(InTitleInfo.mTitleId);
	if (nullptr == titleData)
	{
		GSLOG(Error, TEXT("not find title Data [%d]"), InTitleInfo.mTitleId);
		return;
	}

	if (InTitleInfo.mStatus == TitleStatus::ACTIVED && (titleData->IsNewAcquire() || titleData->IsAcquire()))
	{	// 2022/03/17 PKT - 테스트 중 이미 획득했거나 완료인 상태에서 다시 새롭게 완료 되었다고 오는 경우가 있음.(버그)
		//					방어 코드 넣고 로그 남김.
		GSLOG(Error, TEXT("already title Acquire : [%d]"), InTitleInfo.mTitleId);
		return;
	}

	bool isPrevAcquire = titleData->IsAcquire();
	titleData->UpdateStatus(InTitleInfo.mStatus, InTitleInfo.mConditionCount);

	if (false == isPrevAcquire && true == titleData->IsAcquire())
	{	// 2022/01/21 PKT - 이전에 미획득이였다가 획득 한 경우.
		_collectionRewardData.AddTitle();
	}
	else if (true == isPrevAcquire && false == titleData->IsAcquire())
	{	// 2022/01/21 PKT - 이전에 획득이였다가 미획득 한 경우.
		_collectionRewardData.RemoveTitle();
	}
}

// 2022/01/21 PKT - 칭호 상태(장착 유무) 변경
void FGsTitleManager::PlayerEquipTitle(TitleId InEquipTitle, TitleId InUnEquipTitle)
{
	FGsTitleData* titleData = FindTitleDataInHolder(InUnEquipTitle);
	if (titleData)
	{
		_equipData.UnEquip(titleData);
	}

	titleData = FindTitleDataInHolder(InEquipTitle);
	if (titleData)
	{
		_equipData.Equip(titleData);
	}
}


void FGsTitleManager::RemotePlayerEquipTitle(int64 InGameId, TitleId InFrontId, TitleId InBackId)
{
	UGsGameObjectRemotePlayer* remotePlayer = nullptr;
	FGsGameObjectDataRemotePlayer* remoteData = nullptr;

	if (GSGameObject())
	{
		UGsGameObjectBase* object = GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, InGameId);
		remotePlayer = (nullptr != object) ? Cast<UGsGameObjectRemotePlayer>(object) : nullptr;
		remoteData = (nullptr != remotePlayer) ? remotePlayer->GetCastData<FGsGameObjectDataRemotePlayer>() : nullptr;
	}
	
	if (nullptr == remotePlayer || nullptr == remoteData)
	{
		GSLOG(Error, TEXT("not find Remote Data [%lu]"), InGameId);
		return;
	}

	auto SetTitleSet = [&](TitleId InTitleId, OUT FGsTitlePlayerData::TitleSet& OutTitleSet) -> void
	{
		OutTitleSet = FGsTitlePlayerData::InitData;
		
		if (INVALID_TITLE_ID == InTitleId)
		{
			return;
		}

		const FGsTitleData* titleData = FindTitleDataInHolder(InTitleId);
		if (nullptr == titleData)
		{
			GSLOG(Error, TEXT("not find Title Data [%d]"), InTitleId);
			return;
		}

		OutTitleSet = FGsTitlePlayerData::TitleSet(titleData->GetTitle(), titleData->GetTitleHUDColor());
	};

	FGsTitlePlayerData::TitleSet frontTitleData = FGsTitlePlayerData::InitData;
	FGsTitlePlayerData::TitleSet backTitleData = FGsTitlePlayerData::InitData;

	SetTitleSet(InFrontId, frontTitleData);
	SetTitleSet(InBackId, backTitleData);

	remoteData->SetTitleData(frontTitleData, backTitleData);

	remotePlayer->UpdateBillboard(EGsBillboardUpdateType::Title);
}


FGsTitleData* FGsTitleManager::FindTitleDataInHolder(TitleId InId)
{
	return _titleDataHolder.FindByPredicate([InId](const FGsTitleData& InData)
		{
			return (InData.GetId() == InId);
		}
	);
}

 
int32 FGsTitleManager::GetCategoryCount(TitleEquipSlot InSlot) const
{
	if (false == _categoryDataHolder.Contains(InSlot))
	{
		return 0;
	}

	return _categoryDataHolder[InSlot].Num();
}


bool FGsTitleManager::FindCategoryList(TitleEquipSlot InSlot, OUT TArray<const FGsSchemaTitleSubCategory *>& OutList) const
{
	OutList.Empty();

	if (false == _categoryDataHolder.Contains(InSlot))
	{
		return false;
	}

	OutList.Append(_categoryDataHolder[InSlot]);
	
	return true;
}

bool FGsTitleManager::IsMainCategoryRedDot(TitleEquipSlot InSlot) const
{
	if (false == _categoryDataHolder.Contains(InSlot))
	{
		GSLOG(Error, TEXT("false == _categoryDataHolder.Contains(InSlot)"));
		return false;
	}

	const TArray<const FGsSchemaTitleSubCategory*>& subCategoryList = _categoryDataHolder[InSlot];
	for (const FGsSchemaTitleSubCategory* subCategory : subCategoryList)
	{
		if (_reddotCollector.Contains(subCategory->id))
		{
			if (0 < _reddotCollector[subCategory->id].Num())
			{
				return true;
			}
		}
	}
	return false;
}

bool FGsTitleManager::IsSubCategoryRedDot(TitleEquipSlot InSlot, int32 InIndex) const
{
	if (false == _categoryDataHolder.Contains(InSlot))
	{
		GSLOG(Error, TEXT("false == _categoryDataHolder.Contains(InSlot)"));
		return false;
	}

	const TArray<const FGsSchemaTitleSubCategory*>& subCategoryList = _categoryDataHolder[InSlot];
	if (false == subCategoryList.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == subCategoryList.IsValidIndex(InIndex)"));
		return false;
	}

	TitleCategoryId categoryId = subCategoryList[InIndex]->id;
	if (false == _reddotCollector.Contains(categoryId) || 0 >= _reddotCollector[categoryId].Num())
	{
		return false;
	}

	return true;
}

const TArray<const FGsTitleData*>& FGsTitleManager::MakeViewList(TitleEquipSlot InMainCatetgoryType
	, const int32 InSubCategoryIndex
	, const TArray <TSharedPtr<IGsTitleFilterConditionBase>>& InFilters) const
{
	_viewList.Empty();

	do
	{
		if (false == _categoryDataHolder.Contains(InMainCatetgoryType))
		{
			break;
		}

		const TArray<const FGsSchemaTitleSubCategory* >& subCategoryList = _categoryDataHolder[InMainCatetgoryType];
		if (false == subCategoryList.IsValidIndex(InSubCategoryIndex))
		{
			break;
		}

		TitleCategoryId categoryId = subCategoryList[InSubCategoryIndex]->id;

		if (false == _titleDatasByCategoryId.Contains(categoryId))
		{
			break;
		}

		_viewList.Reserve(_titleDatasByCategoryId[categoryId].Num());
		for (const FGsTitleData* titleData : _titleDatasByCategoryId[categoryId])
		{
			_viewList.Emplace(titleData);
		}
		
		// 2022/01/17 PKT - Filter
		TArray<const FGsTitleData*> tempList;
		for (TSharedPtr<IGsTitleFilterConditionBase> filter : InFilters)
		{
			tempList.Empty(_viewList.Num());
			filter->Execute(_viewList, tempList);
			_viewList = MoveTemp(tempList);
		}

		//**
		// * 2022/01/17 PKT - View Sort Order
		// * 등급 / ID 순 정렬은 로딩 시 미리 해뒀음. RedDot만 해줌 됨.
		// */
		const int32 maxCount = _viewList.Num();

		TArray<const FGsTitleData*> redDotList;
		TArray<const FGsTitleData*> normalList;
		for (const FGsTitleData* data : _viewList)
		{
			if (data->IsNewAcquire())
			{
				redDotList.Emplace(data);
			}
			else
			{
				normalList.Emplace(data);
			}		
		}
		_viewList.Empty(maxCount);
		_viewList.Append(redDotList);
		_viewList.Append(normalList);

	} while (0);

	return _viewList;
}


const FGsTitleData* FGsTitleManager::FindTitleDataInViewListByTitleDataId(TitleId InId) const
{
	int32 findIndex = _viewList.IndexOfByPredicate([InId](const FGsTitleData* InData)
		{
			return InData && InData->GetId() == InId;
		}
	);

	return _viewList.IsValidIndex(findIndex) ? _viewList[findIndex] : nullptr;
}


const TArray<StatType>& FGsTitleManager::GetToHaveStatAll() const
{
	return _titleStatAll;
}


const FGsTitleCollectionReward* FGsTitleManager::GetCollectionRewardData() const
{
	return &_collectionRewardData;
}


const FGsTitleEquip* FGsTitleManager::GetTitleEquipData() const
{
	return &_equipData;
}