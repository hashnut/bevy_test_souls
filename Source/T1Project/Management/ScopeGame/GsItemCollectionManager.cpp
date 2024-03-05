

#include "GsItemCollectionManager.h"
#include "GsUnlockManager.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "Item/Collection/EItemCollectionMainCategory.h"
#include "../Sound/GsSoundPlayer.h"
#include "../ScopeGlobal/GsSoundManager.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGame/GsDepotManager.h"
#include "../Dungeon/GsDungeonMessageHolder.h"
#include "../ScopeGlobal/GsUIManager.h"
#include "../GsMessageHolder.h"
#include "../Message/GsMessageItem.h"
#include "../Message/GsMessageContents.h"
#include "../Message/GsMessageContentHud.h"
#include "../Message/MessageParam/GsItemMessageParam.h"
#include "../Message/MessageParam/GsItemContentsMessageParam.h"
#include "../Message/MessageParam/GsItemCollectionMessageParam.h"
#include "ItemCollection/GsSchemaItemCollection.h"
#include "UI/UIContent/Tray/GsUITrayTickerCollection.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollectionMainCategory.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollectionSubCategoryStatUnit.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "T1Project.h"





/************************************************************************/
/* Passivity Collection                                                 */
/************************************************************************/
void FGsItemCollectionManager::FGsPassivityCollection::Clear(bool InIsMessage /*= true*/)
{
	_passivitys.Empty();

	if (InIsMessage)
	{
		const FGsPrimitiveInt32 Param(StaticCast<int32>(PassivityOwnerType::ITEM_COLLECTION));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &Param);
	}	
}

void FGsItemCollectionManager::FGsPassivityCollection::SendUpdateMessage()
{
	const FGsPrimitiveInt32 Param(StaticCast<int32>(PassivityOwnerType::ITEM_COLLECTION));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &Param);
}

void FGsItemCollectionManager::FGsPassivityCollection::Add(const TArray<int32> InIds, bool InIsMessage /*= true*/)
{
	for (const auto& item : InIds)
	{
		_passivitys.Emplace(item);
	}	

	if (InIsMessage)
	{
		SendUpdateMessage();
	}
}

void FGsItemCollectionManager::FGsPassivityCollection::Remove(const TArray<int32> InIds, bool InIsMessage /*= true*/)
{
	for (const auto& item : InIds)
	{
		if (_passivitys.Contains(item))
		{
			_passivitys.Remove(item);
		}
	}

	if (InIsMessage)
	{
		SendUpdateMessage();
	}
}

const IGsCollectionData* FGsItemCollectionManager::FGsPassivityCollection::GetCollectionData() const
{
	return this;
}
PassivityOwnerType FGsItemCollectionManager::FGsPassivityCollection::GetPassivityOwnerType() const
{
	return PassivityOwnerType::ITEM_COLLECTION;
}
int32 FGsItemCollectionManager::FGsPassivityCollection::GetPassivityCount() const
{
	return _passivitys.Num(); 
}

PassivityId FGsItemCollectionManager::FGsPassivityCollection::GetPassivityId(int32 InIndex) const
{
	return _passivitys.IsValidIndex(InIndex) ? _passivitys[InIndex] : INVALID_PASSIVITY_ID;
}



/************************************************************************/
/* ItemCollection Manager                                               */
/************************************************************************/
using WindowMessageHolder = FGsDungeonMessageHolder;
#define WINDOW_MESSAGE_LOCK {	WindowMessageHolder holer(_messageHolder);
#define WINDOW_MESSAGE_UNLOCK }


void FGsItemCollectionManager::Initialize()
{	
	_messageHolder = 0;

	_categoryDataHolder.Empty();
	_contentsStatAll.Empty();
	_SortDirty.Empty();
	_viewFiledSet.Empty();
	_passivityCollection.Clear(false);
	_fastFinderFieldSet.Empty();
	_validMaterialSet.Empty();
	_fieldGroupBySortOrder.Empty();
	_dataHolder.Empty();	
	/**
	 * 빌드 하는 과정 속에서 Window로 이벤트가 날아갈 수 도 있으므로 이벤트를 막는다.
	 */
	WINDOW_MESSAGE_LOCK	


	BuildCollectionField();

	RegisterMessages();
	
	/**
	* 초기화는 서버에게 패킷 받은 이후 진행 ( Manager Initialize 속도 이슈 관련 중복처리 막기)
	*/
	//for (auto& item : _dataHolder)
	//{
	//	item.Value.OnChangedFieldDelegate.BindRaw(this, &FGsItemCollectionManager::OnChangedFieldDelegate);

	//	//item.Value.Init();
	//}
	WINDOW_MESSAGE_UNLOCK
}

void FGsItemCollectionManager::Finalize()
{
	WINDOW_MESSAGE_LOCK
	_categoryDataHolder.Empty();
	_contentsStatAll.Empty();
	_SortDirty.Empty();
	_viewFiledSet.Empty();
	_passivityCollection.Clear(false);
	_validMaterialSet.Empty();
	_fastFinderFieldSet.Empty();
	_fieldGroupBySortOrder.Empty();
	_dataHolder.Empty();

	UnegisterMessages();

	WINDOW_MESSAGE_UNLOCK

	_messageHolder = 0;
}

void FGsItemCollectionManager::Update(float inTick)
{

}

bool FGsItemCollectionManager::CheckRedDot(bool bInUpdate /*= true*/)
{
	bool bIsRedDot = false;

	do
	{
		// 2022/12/29 PKT - 즉시 등록 가능한 그룹이 있다면 레드닷.
		if (true == _fieldGroupBySortOrder.Contains(ECollectionSortOrder::ImmediatePossible)
			&& 0 < _fieldGroupBySortOrder[ECollectionSortOrder::ImmediatePossible].Num())
		{
			bIsRedDot = true;
		}

	} while (0);

	bool isPrevRedDot = IsRedDot();

	SetRedDot(bIsRedDot);

	if (IsRedDot() != isPrevRedDot)
	{	// 2021/08/13 PKT - RedDot의 변경을 HUD에 알린다.
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::ITEM_COLLECTION, false);
	}

	return IsRedDot();
}

void FGsItemCollectionManager::ClearConditionEntryMaterials()
{
	WINDOW_MESSAGE_LOCK
	
	for (auto& field : _dataHolder)
	{
		field.Value.ClearEntryMaterials();
	}

	WINDOW_MESSAGE_UNLOCK
}

void FGsItemCollectionManager::FixedConditionEntryMaterialInfo(CollectionId InCollectionId, CollectionConditionId InCollectionConditionId, ItemId InMaterialId)
{
	if (_dataHolder.Contains(InCollectionId))
	{
		_dataHolder[InCollectionId].FixedEntryMaterial(InCollectionConditionId, InMaterialId);
	}
}

const TArray<FGsItemCollectionCategory>& FGsItemCollectionManager::CategoryRootDatas() const
{
	if (0 >= _categoryDataHolder.Num())
	{
		do 
		{
			const UGsTableItemCollectionMainCategory* mainCategroryTable = Cast<UGsTableItemCollectionMainCategory>(FGsSchemaItemCollectionMainCategory::GetStaticTable());
			if (nullptr == mainCategroryTable)
			{
				GSLOG(Error, TEXT("nullptr == mainCategroryTable"));
				break;
			}

			TArray<const FGsSchemaItemCollectionMainCategory*> mainCategoryDatas;
			if (false == mainCategroryTable->GetAllRows(mainCategoryDatas))
			{
				GSLOG(Error, TEXT("false == mainCategroryTable->GetAllRows(mainCategoryDatas)"));
				break;
			}

			for (const FGsSchemaItemCollectionMainCategory* item : mainCategoryDatas)
			{
				FGsItemCollectionCategory category;
				category._name = item->name;
				category._category = item;
				// Add..
				_categoryDataHolder.Emplace(category);
			}

			/**
			 * AttainmentUnit sub category
			 */
			{
				const UGsTableItemCollectionSubCategoryAttainmentUnit* subAttainmentUnitCategroryTable
					= Cast<UGsTableItemCollectionSubCategoryAttainmentUnit>(FGsSchemaItemCollectionSubCategoryAttainmentUnit::GetStaticTable());
				if (nullptr == subAttainmentUnitCategroryTable)
				{
					GSLOG(Error, TEXT("nullptr == subAttainmentUnitCategroryTable"));
					break;
				}

				TArray<const FGsSchemaItemCollectionSubCategoryAttainmentUnit*> subAttainmentUnitCategroryData;
				subAttainmentUnitCategroryData.Reserve(subAttainmentUnitCategroryTable->Num());
				if (false == subAttainmentUnitCategroryTable->GetAllRows(subAttainmentUnitCategroryData))
				{
					GSLOG(Error, TEXT("false == subAttainmentUnitCategroryTable->GetAllRows(subAttainmentUnitCategroryData)"));
					break;
				}

				auto mainAttainmentUnitRoot = _categoryDataHolder.FindByPredicate([](const FGsItemCollectionCategory& InData)
					{
						const FGsSchemaItemCollectionMainCategory* schemaData = static_cast<const FGsSchemaItemCollectionMainCategory*>(InData._category);
						return schemaData->type == EItemCollectionMainCategory::AttainmentUnit;
					}
				);

				for (const auto& item : subAttainmentUnitCategroryData)
				{
					FGsItemCollectionCategory category;
					category._name = item->name;
					category._category = item;

					// 2023/1/3 PKT - Root Category
					mainAttainmentUnitRoot->_subCategory.Emplace(category);
				}
			}
			

			/**
			 * StatUnit sub category
			 */
			{
				const UGsTableItemCollectionSubCategoryStatUnit* subStatUnitCategroryTable
					= Cast<UGsTableItemCollectionSubCategoryStatUnit>(FGsSchemaItemCollectionSubCategoryStatUnit::GetStaticTable());
				if (nullptr == subStatUnitCategroryTable)
				{
					GSLOG(Error, TEXT("nullptr == subStatUnitCategroryTable"));
					break;
				}

				TArray<const FGsSchemaItemCollectionSubCategoryStatUnit*> subStatUnitCategroryData;
				subStatUnitCategroryData.Reserve(subStatUnitCategroryTable->Num());
				if (false == subStatUnitCategroryTable->GetAllRows(subStatUnitCategroryData))
				{
					GSLOG(Error, TEXT("false == subStatUnitCategroryTable->GetAllRows(subStatUnitCategroryData)"));
					break;
				}

				auto mainStatUnitRoot = _categoryDataHolder.FindByPredicate([](const FGsItemCollectionCategory& InData)
					{
						const FGsSchemaItemCollectionMainCategory* schemaData = static_cast<const FGsSchemaItemCollectionMainCategory*>(InData._category);
						return schemaData->type == EItemCollectionMainCategory::StatUnit;
					}
				);

				for (const auto& item : subStatUnitCategroryData)
				{
					if (EItemCollectionSubCategoryStatUnit::ALL == item->type)
					{	// 2023/1/18 PKT - 분류별 전체는 제외
						continue;
					}

					FGsItemCollectionCategory category;
					category._name = item->name;
					category._category = item;
					// 2023/1/3 PKT - Root Category
					mainStatUnitRoot->_subCategory.Emplace(category);
				}
			}	
		} while (0);
	}
	return _categoryDataHolder;
}

bool FGsItemCollectionManager::IsCategoryRedDot(EItemCollectionSubCategoryAttainmentUnit InCategory) const
{
	bool outIsRedDot = false;
		
	if (_fieldGroupBySortOrder.Contains(ECollectionSortOrder::ImmediatePossible))
	{
		const auto& fieldArray = _fieldGroupBySortOrder[ECollectionSortOrder::ImmediatePossible].Array();
		if (nullptr != fieldArray.FindByPredicate([InCategory](const FGsItemCollectionField* InData)
			{
				return InData->SchemaData()->AttainmentUnit.GetRow()->type == InCategory;
			}
		))
		{
			outIsRedDot = true;
		}
	}
	return outIsRedDot;
}
	
int32 FGsItemCollectionManager::BuildViewFieldSet(bool InIsBuildSkip, const TSet<ECollectionSortOrder>& InProgressFilter, const TArray<TSharedPtr<PropertyFilter>> InPropertyFilter)
{
	if (InIsBuildSkip)
	{
		return _viewFiledSet.Num();
	}

	_viewFiledSet.Empty();

	// 2022/12/30 PKT - filter에 등록 되어 있는 것들만 선택적으로 정렬함.
	for (auto item : InProgressFilter)
	{
		if (_SortDirty.Contains(item))
		{
			if (_fieldGroupBySortOrder.Contains(item))
			{
				_fieldGroupBySortOrder[item].Sort([](const FGsItemCollectionField& InLeft, const FGsItemCollectionField& InRight) -> bool
					{
						// 2022/12/30 PKT - 현재는 ID순이지만 진행도가 포함 될 예정( 즉시 등록 애들 만.. )
						double leftRatio = InLeft.ProgressRatio();
						double rightRatio = InRight.ProgressRatio();
						if (leftRatio > rightRatio)
						{
							return true;
						}
						else if (leftRatio < rightRatio)
						{
							return false;
						}

						return InLeft.Id() < InRight.Id();
					}
				);
			}
			
			// 2022/12/30 PKT - dirty 초기화
			_SortDirty.Remove(item);
		}
	}
	
	const int32 BeginIndex = StaticCast<int32>(ECollectionSortOrder::ImmediatePossible);
	const int32 EndIndex = StaticCast<int32>(ECollectionSortOrder::Max);

	// 2022/12/30 PKT - 정렬 순서대로 List를 채운다.
	for (int32 idx = BeginIndex; idx < EndIndex; ++idx)
	{
		ECollectionSortOrder order = StaticCast<ECollectionSortOrder>(idx);

		if (false == InProgressFilter.Contains(order))
		{	// 2022/12/30 PKT - filter
			continue;
		}

		if (false == _fieldGroupBySortOrder.Contains(order))
		{
			continue;
		}

		for (const FGsItemCollectionField* item : _fieldGroupBySortOrder[order])
		{
			// 2023/3/31 PKT - 인벤트 날짜 구간에 포함되어 있지 않다면 화면에 출력하지 않는다.
			if (nullptr == item || false == item->IsEnableFlag())
			{
				continue;
			}

			bool isPassed = false;
			for (const auto& filter : InPropertyFilter)
			{
				if (filter.IsValid())
				{
					if (false == filter.Get()->Execute(item))
					{
						isPassed = true;
						break;
					}
				}
			}

			if (false == isPassed)
			{
				_viewFiledSet.Emplace(item);
			}			
		}
	}

	return _viewFiledSet.Num();
}

bool FGsItemCollectionManager::IsValidMaterial(const FGsItem* InMaterial) const
{
	if (InMaterial && IsValidMaterial(InMaterial->GetTID()))
	{
		return true;
	}

	return false;
}

const FGsItemCollectionManager::FGsPassivityCollection* FGsItemCollectionManager::PassivityCollectionData() const
{
	return &_passivityCollection;
}

const FGsItemCollectionField* FGsItemCollectionManager::FindFieldById(CollectionId InId) const
{
	return _dataHolder.Contains(InId) ? &(_dataHolder[InId]) : nullptr;
}

const FGsItemCollectionField* FGsItemCollectionManager::FindFieldByConditionId(CollectionConditionId InId) const
{
	CollectionId collectionId = FastFindFieldIdByConditionId(InId);
	return FindFieldById(collectionId);
}

void FGsItemCollectionManager::FieldAll(TArray<const FGsItemCollectionField*>& OutData) const
{
	OutData.Empty();

	for (const auto& item : _dataHolder)
	{
		if (false == item.Value.IsEnableFlag())
		{
			continue;
		}

		OutData.Emplace(&item.Value);
	}
}

bool FGsItemCollectionManager::IsValidMaterial(ItemId InMaterial) const
{
	if (false == _validMaterialSet.Contains(InMaterial))
	{
		return false;
	}

	for (const auto& item : _validMaterialSet[InMaterial])
	{
		if (_dataHolder.Contains(item) && _dataHolder[item].IsEnableFlag())
		{
			return true;
		}
	}

	return false;
}

TArray<const FGsItemCollectionCondition*> FGsItemCollectionManager::FindConditionsByItemId(const class FGsItem* InMaterial) const
{
	ItemId materialId = (InMaterial) ? InMaterial->GetTID() : INVALID_ITEM_ID;

	return FindConditionsByItemId(materialId);
}

TArray<const FGsItemCollectionCondition*> FGsItemCollectionManager::FindConditionsByItemId(ItemId InMaterial) const
{
	TArray<const FGsItemCollectionCondition*> OutSet;
	OutSet.Empty();

	using ConditionMaterial = FGsItemCollectionCondition::CollectMaterialData;

	if (true == _validMaterialSet.Contains(InMaterial))
	{
		for (const auto& item : _validMaterialSet[InMaterial])
		{
			if (_dataHolder.Contains(item) && _dataHolder[item].IsEnableFlag())
			{
				const FGsItemCollectionField& fieldData = _dataHolder[item];

				for (const auto& condition : fieldData.Conditions())
				{
					// 2023/11/28 PKT - https://jira.com2us.com/jira/browse/C2URWQ-7468
					//bool isFindData = false;

					for (const auto& material : condition.MaterialAll())
					{
						if (material.Key == InMaterial)
						{
							OutSet.Emplace(&condition);
						//	isFindData = true;
							break;
						}
					}

					/*if (isFindData)
					{
						break;
					}*/
				}
			}
		}
	}

	return OutSet;
}

TSet<StatType>& FGsItemCollectionManager::ContentsStatAll() const
{
	if (0 >= _contentsStatAll.Num())
	{
		for (const auto& item : _dataHolder)
		{
			// 2024/1/17 PKT - 이벤트/기간제 무시하고 모두 가져 온다.
			for (StatType stat : item.Value.StatTypeSet())
			{
				_contentsStatAll.Emplace(stat);
			}
		}
	}

	return _contentsStatAll;
}

void FGsItemCollectionManager::NotifyInit(const TArray<CollectionData>& InCompletedFieldDataSet, const TArray<CollectionConditionData> InProgressConditions, const TArray<CollectionId> InBookMarkFieldIds)
{
	// 2022/12/29 PKT - 셋팅 과정에서 발생 될 수 있는 메세지를 Lock( 대량으로 메세지가 통보 될 수 있기 때문에 )
	WINDOW_MESSAGE_LOCK

	// 2023/7/11 PKT - Reconnect 에 의해 이곳은 중복해서 호출 될 수 있다. 호출 될때마다 Data를 갱신 하기 위해 모든 Data 초기화
	_passivityCollection.Clear(false);
	_SortDirty.Empty();
	_fieldGroupBySortOrder.Empty();

	for (auto& data : _dataHolder)
	{
		if (false == data.Value.OnChangedFieldDelegate.IsBound())
		{
			data.Value.OnChangedFieldDelegate.BindRaw(this, &FGsItemCollectionManager::OnChangedFieldDelegate);
		}		
		data.Value.Init();

	}

	/**
	 * 완료 목록
	 */
	for (const auto& item : InCompletedFieldDataSet)
	{
		if (false == _dataHolder.Contains(item.mCollectionId))
		{
			GSLOG(Error, TEXT("false == _dataHolder.Contains(item.mCollectionId) : [%d]"), item.mCollectionId);
			continue;
		}

		_dataHolder[item.mCollectionId].EventEnableFlag(true);
		_dataHolder[item.mCollectionId].ForceCompleted();
		_dataHolder[item.mCollectionId].SetEndTime(item.mExpiredTime);

		// 2023/1/6 PKT - 패시비티 추가
		_passivityCollection.Add(_dataHolder[item.mCollectionId].PassivitySet(), false);
	}

	/**
	 * 진행 중인 목록
	 */
	for (const auto& item : InProgressConditions)
	{
		CollectionId fieldId = FastFindFieldIdByConditionId(item.mCollectionConditionId);
		if (false == _dataHolder.Contains(fieldId))
		{
			GSLOG(Error, TEXT("false == _dataHolder.Contains(fieldId) : [%d]"), fieldId);
			continue;
		}


		_dataHolder[fieldId].EventEnableFlag(true);
		_dataHolder[fieldId].Register(item.mCollectionConditionId, item.mRegisterCount);
	}

	/**
	 * 즐겨 찾기 목록
	 */
	for (const auto& item : InBookMarkFieldIds)
	{
		if (false == _dataHolder.Contains(item))
		{
			GSLOG(Error, TEXT("false == _dataHolder.Contains(item) : [%d]"), item);
			continue;
		}

		_dataHolder[item].EventEnableFlag(true);
		_dataHolder[item].BookMarkEnable(true);
	}


	for (auto& pairData : _validMaterialSet)
	{
		if (0 < GItem()->FindByTID(pairData.Key).Num())
		{
			for (CollectionId collectionId : pairData.Value)
			{
				if (_dataHolder.Contains(collectionId))
				{
					_dataHolder[collectionId].Update();
				}
			}
		}
	}
	
	WINDOW_MESSAGE_UNLOCK

	// 2023/1/6 PKT - Passivity Collection Update
	_passivityCollection.SendUpdateMessage();

	// 2023/1/6 PKT - Item Collection Window Update Pass( InGame 때 오는 패킷으로. 업데이트 할 필요 없음. )
}

void FGsItemCollectionManager::AckUpdateField(CollectionData InFieldData, CollectionConditionId InConditionId, int32 InCount)
{
	// 2022/12/30 PKT - 완료 목록에 따른 진행 현황을 업데이트 함.
	if (false == _dataHolder.Contains(InFieldData.mCollectionId))
	{
		GSLOG(Error, TEXT("_dataHolder.Contains(InFieldData.mCollectionId) : [%d]"), InFieldData.mCollectionId);
		return;
	}

	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (soundPlayer)
	{
		soundPlayer->PlaySound2D("UI_Collection_Set");
	}

	_dataHolder[InFieldData.mCollectionId].Register(InFieldData.mExpiredTime, InConditionId, InCount);

	if (ECollectionSortOrder::Completed == _dataHolder[InFieldData.mCollectionId].SortOrder())
	{
		// 2023/1/6 PKT - 패시비티 추가
		_passivityCollection.Add(_dataHolder[InFieldData.mCollectionId].PassivitySet());

		// 2022/12/29 PKT - 완료 했다면 완료 티커
		UGsUITrayTickerCollection* ticker = Cast<UGsUITrayTickerCollection>(GUI()->OpenAndGetWidget(TEXT("TrayTickerCollection")).Get());
		if (ticker)
		{
			const UGsUITrayTickerCollection::FInitData InitData
			{
				_dataHolder[InFieldData.mCollectionId].Title()
				, _dataHolder[InFieldData.mCollectionId].StatSetText()
			};

			ticker->SetData(InitData);
		};
	}
}

void FGsItemCollectionManager::AckBookMark(const CollectionId InFieldId, bool InIsEnable)
{
	if (false == _dataHolder.Contains(InFieldId))
	{
		GSLOG(Error, TEXT("false == _dataHolder.Contains(InFieldId) : [%d]"), InFieldId);
		return;
	}

	// 2022/12/29 PKT - Book Mark 상태 변경
	_dataHolder[InFieldId].BookMarkEnable(InIsEnable);
}

void FGsItemCollectionManager::AckEventChanged(const CollectionId InFieldId, bool InIsEnable)
{
	if (false == _dataHolder.Contains(InFieldId))
	{
		GSLOG(Error, TEXT("false == _dataHolder.Contains(InFieldId) : [%d]"), InFieldId);
		return;
	}

	if (_dataHolder[InFieldId].IsEventField() || _dataHolder[InFieldId].IsHaveLimitTime())
	{
		// 2023/7/18 PKT - https://jira.com2us.com/jira/browse/C2URWQ-6056
		//				   패시비티 부터 제거 후 상태를 변경 해야 한다. 안그럼 위의 이슈 생김

		bool isPrevEnvenEnable = _dataHolder[InFieldId].IsEnableFlag();
		
		// 2023/3/31 PKT - Passivity Remove
		if (false == InIsEnable && ECollectionSortOrder::Completed == _dataHolder[InFieldId].SortOrder())
		{
			_passivityCollection.Remove(_dataHolder[InFieldId].PassivitySet());
		}
		
		if (false == InIsEnable)
		{
			// 2023/9/7 PKT - 기존 이벤트 기간 이였던 애들 중 이벤트 기간이 끝난 애들에 대한 Ticker
			if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_COLLECTION) && isPrevEnvenEnable && false == _dataHolder[InFieldId].Title().IsEmpty())
			{
				// 2023/9/5 PKT - [{0}] 이벤트 기간이 만료되었습니다.
				FText format;
				FText::FindText(TEXT("ItemCollectionMainText"), TEXT("Collection_Ticker_Event_Expired"), format);
				FString msg = FString::Format(*format.ToString(), { *_dataHolder[InFieldId].Title().ToString() });
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::FromString(msg), TrayCondition::TC_Active, SectionPriority::Insert);
			}
		}

		if (_dataHolder[InFieldId].IsEventField())
		{
			_dataHolder[InFieldId].EventEnableFlag(InIsEnable);
		}
		else if (_dataHolder[InFieldId].IsHaveLimitTime() && false == InIsEnable)
		{
			_dataHolder[InFieldId].UnRegister();
		}		
	}
}

const TArray<const FGsItemCollectionField*>& FGsItemCollectionManager::ViewFieldSet() const
{
	return _viewFiledSet;
}

void FGsItemCollectionManager::RegisterMessages()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	// 2021/08/02 PKT - inventory Item - New ..
	_listItemActionDelegates.Emplace(messageHolder->GetItem().AddRaw(MessageItem::ADDITEMLIST
		, this
		, &FGsItemCollectionManager::OnAddToInventory));
	// 2021/08/02 PKT - inventory Item - Update ..
	_listItemActionDelegates.Emplace(messageHolder->GetItem().AddRaw(MessageItem::UPDATEITEM
		, this
		, &FGsItemCollectionManager::OnChangeToInventory));
	// 2021/08/02 PKT - inventory Item - Remove ..
	_listItemActionDelegates.Emplace(messageHolder->GetItem().AddRaw(MessageItem::REMOVEITEM
		, this
		, &FGsItemCollectionManager::OnRemoveToInventory));

	// 2021/08/02 PKT - Depot Item - New ..
	_listContentItemActionDelegates.Emplace(messageHolder->GetItemContents().AddRaw(MessageContentItem::DEPOT_ADD_ITEM
		, this
		, &FGsItemCollectionManager::OnAddToDepot));
	// 2021/08/02 PKT - Depot Item - Update ..
	_listContentItemActionDelegates.Emplace(messageHolder->GetItemContents().AddRaw(MessageContentItem::DEPOT_INVALIDATE
		, this
		, &FGsItemCollectionManager::OnChangeToDepot));
	// 2021/08/02 PKT - Depot Item - Remove ..
	_listContentItemActionDelegates.Emplace(messageHolder->GetItemContents().AddRaw(MessageContentItem::DEPOT_REMOVE_ITEM
		, this
		, &FGsItemCollectionManager::OnRemoveToDepot));
}

void FGsItemCollectionManager::UnegisterMessages()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	for (const TPair<MessageItem, FDelegateHandle>& item : _listItemActionDelegates)
	{
		messageHolder->GetItem().Remove(item);
	}
	_listItemActionDelegates.Empty();

	for (const TPair<MessageContentItem, FDelegateHandle>& item : _listContentItemActionDelegates)
	{
		messageHolder->GetItemContents().Remove(item);
	}
	_listContentItemActionDelegates.Empty();
}

void FGsItemCollectionManager::OnAddToInventory(FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* param = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == param)
	{
		return;
	}

	/**
	 * 아이템을 이전에는 한번의 이벤트로 하나의 아이템만을 처리 하였음. 하지만 이제 몇백개씩의 아이템을 처리해야 하는 경우가 생겼음. ( BM 관련.// )
	 * 아이템 추가는 메세지를 모았다가 필요할 경우 한번에 보내주는 방식으로 변경 해야 할 수 도 있음.
	 */
	bool isChangedCost = false;

	//WINDOW_MESSAGE_LOCK	
	TWeakObjectPtr<UGsItemManager> itemManager = GItem();
	for (int32 idx = 0; idx < param->_itemDbidList.Num(); ++idx)
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(param->_itemDbidList[idx]);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID : %lld"), param->_itemDbidList[idx]);
			return;
		}

		isChangedCost |= ChangedCost(item.Pin().Get());
	}
	//WINDOW_MESSAGE_UNLOCK

	//if (isChangedCost)
	//{
	//	SendWindowMessage(INVALID_COLLECTION_ID);
	//}
}

void FGsItemCollectionManager::OnChangeToInventory(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	TWeakObjectPtr<UGsItemManager> itemManager = GItem();
	if (itemManager.IsValid())
	{
		TWeakPtr<FGsItem> findData = itemManager->FindItem(Param->_itemDBID);
		if (false == findData.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID : %lld"), Param->_itemDBID);
			return;
		}

		ChangedCost(findData.Pin().Get());
	}
}

void FGsItemCollectionManager::OnRemoveToInventory(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	ChangedCost(Param->_itemTID, Param->_itemDBID);
}

void FGsItemCollectionManager::OnAddToDepot(const IGsMessageParam* InParam)
{
	const FGsAddDepotItem* Param = StaticCast<const FGsAddDepotItem*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	FGsDepotManager* depotManager = GSDepot();
	if (depotManager)
	{
		TWeakPtr<const FGsItem> findData = depotManager->FindDepotItemByDBID(Param->_itemDBID);
		if (false == findData.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}

		ChangedCost(findData.Pin().Get());
	}
}

void FGsItemCollectionManager::OnChangeToDepot(const IGsMessageParam* InParam)
{
	const FGsUpdateDepotItem* Param = StaticCast<const FGsUpdateDepotItem*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	FGsDepotManager* depotManager = GSDepot();
	if (depotManager)
	{
		TWeakPtr<const FGsItem> findData = depotManager->FindDepotItemByDBID(Param->_itemDBID);
		if (false == findData.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}
		
		ChangedCost(findData.Pin().Get());
	}
}

void FGsItemCollectionManager::OnRemoveToDepot(const IGsMessageParam* InParam)
{
	const FGsRemoveDepotItem* Param = StaticCast<const FGsRemoveDepotItem*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	ChangedCost(Param->_itemTID, Param->_itemDBID);
}

bool FGsItemCollectionManager::ChangedCost(const FGsItem* InCost)
{
	// 2023/5/8 PKT - 세진님 曰 " 아이템 상태 변경 시(장비 착용/미착용) 등에 의해 렉이 발생. 이쪽으로 의심됨. 이것에 대한 사유로 코드 수정 함.
	if (nullptr == InCost || false == _validMaterialSet.Contains(InCost->GetTID()))
	{
		return false;
	}

	bool isChangedCost = false;

	const auto& CollectionIdSet = _validMaterialSet[InCost->GetTID()];
	for (const auto& collectionId : CollectionIdSet)
	{
		if (_dataHolder.Contains(collectionId))
		{
			isChangedCost |= _dataHolder[collectionId].UpdateMaterial(InCost);
		}
	}
	
	return isChangedCost;
}

void FGsItemCollectionManager::ChangedCost(ItemId InCostId, ItemDBId InCostDBId)
{
	// 2023/5/8 PKT - 세진님 曰 " 아이템 상태 변경 시(장비 착용/미착용) 등에 의해 렉이 발생. 이쪽으로 의심됨. 이것에 대한 사유로 코드 수정 함.
	if (INVALID_ITEM_GROUP_ID == InCostId || false == _validMaterialSet.Contains(InCostId))
	{
		return;
	}

	const auto& CollectionIdSet = _validMaterialSet[InCostId];
	for (const auto& collectionId : CollectionIdSet)
	{
		if (_dataHolder.Contains(collectionId))
		{
			_dataHolder[collectionId].RemoveMaterial(InCostId, InCostDBId);
		}
	}

	/*for (auto& item : _dataHolder)
	{
		item.Value.RemoveMaterial(InCostId, InCostDBId);
	}*/
}

void FGsItemCollectionManager::OnChangedFieldDelegate(const FGsItemCollectionField* InField, const ECollectionSortOrder InPrevSortOrder)
{
	if (nullptr == InField)
	{
		GSLOG(Error, TEXT("nullptr == InField"));
		return;
	}

	// 2022/12/29 PKT - 이전 그룹에서 삭제
	if (InField->SortOrder() != InPrevSortOrder && _fieldGroupBySortOrder.Contains(InPrevSortOrder))
	{		
		_fieldGroupBySortOrder[InPrevSortOrder].Remove(InField);
	}
	else if (false == InField->IsEnableFlag() && _fieldGroupBySortOrder.Contains(InPrevSortOrder))
	{	// 2023/3/31 PKT - 비활성화 되어 있다면 삭제
		_fieldGroupBySortOrder[InPrevSortOrder].Remove(InField);
	}

	// 2022/12/29 PKT - 처음에는 그룹에 아무것도 들어있지 않기 때문에 무조건 추가 한다. 이미 있으면 추가 되지 않음.(TSet)
	_fieldGroupBySortOrder.FindOrAdd(InField->SortOrder()).Emplace(InField);

	// 2022/12/29 PKT - Field 상태가 변경 되었으므로 정렬이 필요하다.( 활성화 되어 있는 것들 만..)
	if (InField->IsEnableFlag())
	{
		_SortDirty.Emplace(InField->SortOrder());
	}	

	// 2022/12/29 PKT - Send Message
	SendWindowMessage(InField->Id());
}

void FGsItemCollectionManager::SendWindowMessage(CollectionId InId)
{
	// 2022/12/29 PKT - RedDot Update
	CheckRedDot();

	const static int32 IS_SEND_MESSAGE_UNLOCK = 0;
	if (IS_SEND_MESSAGE_UNLOCK != _messageHolder)
	{	// 2022/10/11 PKT - Message Hold 상태
		return;
	}

	const FGsItemCollectionMessageParam param(InId);
	GMessage()->GetItemCollection().SendMessage(MessageContentItemCollection::FIELD_INVALID, &param);
}

CollectionId FGsItemCollectionManager::FastFindFieldIdByConditionId(CollectionConditionId InId) const
{
	return _fastFinderFieldSet.Contains(InId) ? _fastFinderFieldSet[InId] : INVALID_COLLECTION_CONDITION_ID;
}

void FGsItemCollectionManager::BuildCollectionField()
{
	const TMap<FName, uint8*>* RowMap = nullptr;

	FDateTime StartTime = FDateTime::UtcNow();

	const UGsTableItemCollection* table = Cast<UGsTableItemCollection>(FGsSchemaItemCollection::GetStaticTable());
	if (nullptr == table || false == table->GetRowMap(RowMap))
	{
		GSLOG(Error, TEXT("nullptr == table || false == table->GetRowMap(RowMap)"));
		return;
	}

	FDateTime EndTime = FDateTime::UtcNow();
	int64 Milliseconds = (EndTime.ToUnixTimestamp() * 1000 + EndTime.GetMillisecond()) - (StartTime.ToUnixTimestamp() * 1000 + StartTime.GetMillisecond());
	FString TimestampString = FString::Printf(TEXT("%lld milliseconds"), Milliseconds);
	GSLOG(Error, TEXT("FGsItemCollectionManager::Initialize() : %s"), *TimestampString);

	_dataHolder.Reserve(RowMap->Num());

	for (auto& item : *RowMap)
	{
		const FString RowName = item.Key.ToString();
		if (false == RowName.IsNumeric())
		{
			GSLOG(Warning, TEXT("false == RowName.IsNumeric(), RowName:%s"), *RowName);
			continue;
		}

		// 2022/12/29 PKT - Create Collection Field
		const CollectionId filedId = FCString::Atoi(*RowName);

		FGsItemCollectionField newField(filedId, reinterpret_cast<const FGsSchemaItemCollection*>(item.Value));

		/**
		 * 빠른 정보 찾기를 위한 Build
		 */
		for (const auto& condtion : newField.Conditions())
		{
			_fastFinderFieldSet.FindOrAdd(condtion.Id()) = filedId;

			for (const auto& material : condtion.MaterialAll())
			{
				_validMaterialSet.FindOrAdd(material.Key).Emplace(filedId);
			}
		}

		 //2023/1/11 PKT - Add
		_dataHolder.Emplace(filedId, newField);		
	}
}