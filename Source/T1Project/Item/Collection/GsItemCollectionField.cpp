

#include "GsItemCollectionField.h"

#include "GsTableManager.h"

#include "../GsItemManager.h"
#include "../Management/ScopeGame/GsValidatorChainManager.h"

#include "../UTIL/GsTimeSyncUtil.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollection.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "../UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

#include "T1Project.h"




FGsItemCollectionField::FGsItemCollectionField(const CollectionId InId, const FGsSchemaItemCollection* InSchema) 
	: _schemaData(InSchema), _id(InId), _endDate(0)//, _isEnableFlag(_schemaData->eventType == ItemCollectionEventType::NORMAL)
{
	/**
	 * Collection Filed
	 */
	BuildConditionSet();
	BuildValidDateRange(_validDateRange);

	// 2023/3/31 PKT - Event는 기본적으로 꺼준다.(서버에서 따로 내려와서 활성화)
	_isEnableFlag = (false == IsEventField() || false == _haveValidDateRange);
}

void FGsItemCollectionField::Init()
{
	// 2022/12/29 PKT - 기본 상태
	_sortOrder = ECollectionSortOrder::Idel;

	_serachKeyWord.Empty();

	_bookMarkFlag = false;

	//static Currency RESET_REGISTER_AMOUNT = 0;

	for (auto& item : _conditionSet)
	{
		if (false == item.OnChangedConditionStateDelegate.IsBound())
		{
			item.OnChangedConditionStateDelegate.BindRaw(this, &FGsItemCollectionField::OnChangedCondition);
		}

		//item.Register(RESET_REGISTER_AMOUNT);
		item.Init();
	}

	OnChangedCondition();
}

void FGsItemCollectionField::Update()
{
	if (ECollectionSortOrder::Completed == _sortOrder)
	{
		return;
	}

	for (FGsItemCollectionCondition& condition : _conditionSet)
	{
		if (ECollectionMaterialActionType::Completed != condition.ActionType())
		{
			condition.Register(condition.RegisterCount());
		}		
	}
}

void FGsItemCollectionField::ForceCompleted()
{
	for (auto& item : _conditionSet)
	{
		item.Register(item.ConditionCount());
	}
}

void FGsItemCollectionField::Register(int64 InEndDate, CollectionConditionId InId, Currency InRegisterCount)
{
	SetEndTime(InEndDate);	

	auto findData = FindCondition(InId);
	if (findData)
	{
		// 2022/12/29 PKT - 재료 상태 업데이트
		findData->Register(InRegisterCount);
	}
}

void FGsItemCollectionField::Register(CollectionConditionId InId, Currency InRegisterCount)
{
	auto findData = FindCondition(InId);
	if (findData)
	{
		// 2022/12/29 PKT - 재료 상태 업데이트
		findData->Register(InRegisterCount);
	}
}

void FGsItemCollectionField::UnRegister()
{
	for (auto& item : _conditionSet)
	{
		item.UnRegister();
	}
}

void FGsItemCollectionField::SetEndTime(int64 InEndTime)
{
	_endDate = InEndTime;
}

void FGsItemCollectionField::BookMarkEnable(bool InEnable)
{
	if (_bookMarkFlag != InEnable)
	{
		_bookMarkFlag = InEnable;
		// 2022/12/29 PKT - 북마크에 따른 Sort 타입은 변경이 없으므로 현재의 Sort 타입을 넣어줘도 무관하다.
		if (OnChangedFieldDelegate.IsBound())
		{
			OnChangedFieldDelegate.Execute(this, _sortOrder);
		}		
	}
}

void FGsItemCollectionField::EventEnableFlag(bool InEnable)
{
	// 2023/1/10 PKT - 유효 기간을 갖고 있을 경우에만 변경
	if (IsEventField() && _isEnableFlag != InEnable)
	{
		_isEnableFlag = InEnable;

		// 2023/1/10 PKT - 조건들 검사로 필드의 상태를 다시 계산
		OnChangedCondition();
	}
}

bool FGsItemCollectionField::IsHaveLimitTime() const
{
	const static int32 ZERO_TIME = 0;
	return ZERO_TIME != _schemaData->durationTime;
}

void FGsItemCollectionField::ClearEntryMaterials()
{
	for (auto& item : _conditionSet)
	{
		item.FixedEntryMaterial(INVALID_ITEM_ID);
	}
}

void FGsItemCollectionField::FixedEntryMaterial(CollectionConditionId InId, ItemId InMaterialId)
{
	for (auto& item : _conditionSet)
	{
		if (InId == item.Id())
		{
			item.FixedEntryMaterial(InMaterialId);
		}
	}
}

bool FGsItemCollectionField::UpdateMaterial(const class FGsItem* InMaterial)
{
	bool isChangedCost = false;
	for (auto& item : _conditionSet)
	{
		isChangedCost |= item.DetectChangeCost(InMaterial);
	}
	return isChangedCost;
}

void FGsItemCollectionField::RemoveMaterial(ItemId InMaterialId, ItemDBId InMaterialDBId)
{
	for (auto& item : _conditionSet)
	{
		item.DetectChangeCost(InMaterialId, InMaterialDBId);
	}
}

void FGsItemCollectionField::OnChangedCondition()
{
	// 2022/12/30 PKT - 이전 상태 백업
	ECollectionSortOrder prevSortOrder = _sortOrder;
	// 2022/12/30 PKT - 상태 초기화
	_sortOrder = ECollectionSortOrder::Max;

	do
	{
		if (false == IsEnableFlag())
		{
			// 2022/12/30 PKT - 유효한 컬렉션 필드가 아님!!
			_sortOrder = ECollectionSortOrder::InActive;
			break;
		}

		auto ChangeEnum = [](ECollectionMaterialActionType InState)->ECollectionSortOrder
		{
			switch (InState)
			{
			case ECollectionMaterialActionType::ImmediatePossible:
				return ECollectionSortOrder::ImmediatePossible;
			case ECollectionMaterialActionType::ConditionalPossible:
				return ECollectionSortOrder::ConditionalPossible;
			case ECollectionMaterialActionType::Impossible:
				return ECollectionSortOrder::Idel;
			case ECollectionMaterialActionType::Completed:
				return ECollectionSortOrder::Completed;
			}
			return ECollectionSortOrder::Max;
		};

		bool isProgress = false;

		for (const auto& item : _conditionSet)
		{
			ECollectionSortOrder state = ChangeEnum(item.ActionType());

			if (state < _sortOrder)
			{	// 2022/12/27 PKT - 오름 차순( 화면에 보이는 기준. )
				_sortOrder = state;
			}

			if (state == ECollectionSortOrder::Completed)
			{	// 2022/12/27 PKT - 하나라도 완료 한 것이 있다면 진행 중으로 간주...
				isProgress = true;
			}
		}

		if (isProgress && ECollectionSortOrder::Idel == _sortOrder)
		{	// 2022/12/30 PKT - 아무것도 아닌 상태에서 하나라도 등록이 되어 있다면 진행 중 으로 처리
			_sortOrder = ECollectionSortOrder::Progressing;
		}

	} while (0);

	// 2022/12/27 PKT - 우선 순위가 변경 되지 않아도. 화면상에서는 갱신 되어야 할 필요가 있으므로, 무조건 갱신 요청 함.

	// 2022/12/28 PKT - 관리자에게 변경 되었음을 알린다.
	if (OnChangedFieldDelegate.IsBound())
	{
		OnChangedFieldDelegate.Execute(this, prevSortOrder);
	}
}

FGsItemCollectionCondition* FGsItemCollectionField::FindCondition(CollectionConditionId InId)
{
	return _conditionSet.FindByPredicate([InId](const FGsItemCollectionCondition& InData)
		{
			return InId == InData.Id();
		}
	);
}

const FGsSchemaItemCollection* FGsItemCollectionField::SchemaData() const
{
	return _schemaData;
}

CollectionId FGsItemCollectionField::Id() const
{
	return _id;
}

const FText& FGsItemCollectionField::Title() const
{
	return _schemaData->name;
}

ECollectionSortOrder FGsItemCollectionField::SortOrder() const
{
	return _sortOrder;
}

bool FGsItemCollectionField::IsBookMarkEnable() const
{
	return _bookMarkFlag;
}

bool FGsItemCollectionField::IsEventField() const
{
	return (ItemCollectionEventType::EVENT == _schemaData->eventType);
}

bool FGsItemCollectionField::IsEnableFlag() const
{
	return _isEnableFlag;
}

const FText FGsItemCollectionField::TextDateTime() const
{
	FText outText = FText::GetEmpty();

	if (IsEventField())
	{
		outText = ValidDateRangeText();
	}
	else if (IsHaveLimitTime())
	{
		outText = HaveLimitTimeText();
	}

	return outText;
}

const TArray<FGsItemCollectionCondition>& FGsItemCollectionField::Conditions() const
{
	return _conditionSet;
}

const TArray<int32>& FGsItemCollectionField::PassivitySet() const
{
	return _schemaData->passivityIds;
}

const TArray<StatType> FGsItemCollectionField::StatTypeSet() const
{
	if (0 >= _statTypeSet.Num())
	{
		_statTypeSet.Empty();

		do 
		{
			const UGsTablePassivitySet* table = Cast<UGsTablePassivitySet>(FGsSchemaPassivitySet::GetStaticTable());
			if (nullptr == table)
			{
				GSLOG(Error, TEXT("nullptr == table"));
				break;
			}

			for (auto item : _schemaData->passivityIds)
			{
				const FGsSchemaPassivitySet* schemaPassivitySet = nullptr;
				if (false == table->FindRowById(item, schemaPassivitySet))
				{
					GSLOG(Warning, TEXT("false == table->FindRowById(item, schemaPassivitySet) : %d "), item);
					continue;
				}

				FGsEffectStringHelper::FGsEffectAddData EffectAddData;
				if (false == FGsEffectStringHelper::AddPassivityEffect(schemaPassivitySet, EffectAddData))
				{
					if (0 < schemaPassivitySet->effectInfoList.Num())
					{
						int effectId = schemaPassivitySet->effectInfoList[0].effectId;
						GSLOG(Warning, TEXT("!FGsEffectStringHelper::AddPassivityEffect(PassivitySet, EffectAddData), id:%d, type:%d, effectId:%d")
							, schemaPassivitySet->id, schemaPassivitySet->type, effectId);
					}
					continue;
				}

				for (auto& stat : EffectAddData._statMap)
				{	// 2021/09/23 PKT - Add Stat Info
					_statTypeSet.Emplace(stat.Key);
				}
			}

		} while (0);		
	}

	return _statTypeSet;
}

const FText& FGsItemCollectionField::StatSetText() const
{
	if (0 < _schemaData->passivityIds.Num() && _statText.IsEmpty())
	{
		FString buffer;

		do
		{
			for (auto item : _schemaData->passivityIds)
			{
				FString text;
				if (false == FGsEffectStringHelper::GetPassivityEffectString(item, text))
				{
					GSLOG(Error, TEXT("false == FGsEffectStringHelper::GetPassivityEffectString(item, text) : %d"), item);
					continue;
				}

				buffer.Append(text);
			}
		} while (0);

		_statText = FText::FromString(buffer);
	}

	return _statText;
}

bool FGsItemCollectionField::ContainsSerachWord(const FString& InKeyWord) const
{
	for (const auto& item : _conditionSet)
	{
		if (item.ContainsSerachWord(InKeyWord))
		{
			return true;
		}
	}

	if (_serachKeyWord.IsEmpty())
	{
		_serachKeyWord = _schemaData->name.ToString();
		_serachKeyWord.ReplaceInline(TEXT(" "), TEXT(""));
	}

	return _serachKeyWord.Contains(InKeyWord);
}

double FGsItemCollectionField::ProgressRatio() const
{
	if (ECollectionSortOrder::ImmediatePossible != _sortOrder)
	{	// 2023/1/5 PKT - 즉시 등록이 일때만 적용 되는 규칙임.
		return 0;
	}

	double ImmediateCount = 0;
	double UnCompletedCount = 0;
	for (const auto& item : _conditionSet)
	{
		if (ECollectionMaterialActionType::ImmediatePossible == item.ActionType())
		{	// 2023/1/5 PKT - 즉시 등록 가능 한 슬롯
			++ImmediateCount;
		}
		
		if (ECollectionMaterialActionType::Completed != item.ActionType())
		{	// 2023/1/5 PKT - 등록 되지 않은 슬롯( 즉시 등록 가능 슬롯 포함 )
			++UnCompletedCount;
		}
	}

	// 2023/1/6 PKT - maxCount = (ImmediateCount + UnCompletedCount)
	return UnCompletedCount <= 0 ? 0 : (ImmediateCount / UnCompletedCount);
}

void FGsItemCollectionField::BuildConditionSet()
{
	_conditionSet.Empty();

	/*auto MakeCondition = [](int32 InConditionId, const TArray<int32>& InConditionItemSet, const TArray<int32>& InConditionLevelSet, int32 InItemCount, TArray<FGsItemCollectionCondition>& OutConditionSet) -> bool
	{
		if (0 >= InConditionId || 0 >= InConditionItemSet.Num() || 0 >= InConditionLevelSet.Num() || 0 >= InItemCount)
		{
			return false;
		}

		TArray<FGsItemCollectionCondition::CollectMaterialData> materialDataSet;
		for (int32 idx = 0; idx < InConditionItemSet.Num(); ++idx)
		{
			FGsItemCollectionCondition::CollectMaterialData materialData;
			materialData.Key = InConditionItemSet[idx];
			materialData.Value = InConditionLevelSet.IsValidIndex(idx) ? InConditionLevelSet[idx] : 0;
			materialDataSet.Emplace(materialData);
		}

		OutConditionSet.Emplace(FGsItemCollectionCondition(InConditionId, materialDataSet, InItemCount));

		return true;
	};

	do 
	{
		if (nullptr == _schemaData)
		{
			GSLOG(Error, TEXT("nullptr == _schemaData"));
			break;
		}

		if (false == MakeCondition(_schemaData->slotItemConditionId0, _schemaData->slotItemIds0, _schemaData->slotItemLevels0, _schemaData->slotItemCount0, _conditionSet))
		{
			break;
		}

		if (false == MakeCondition(_schemaData->slotItemConditionId1, _schemaData->slotItemIds1, _schemaData->slotItemLevels1, _schemaData->slotItemCount1, _conditionSet))
		{
			break;
		}

		if (false == MakeCondition(_schemaData->slotItemConditionId2, _schemaData->slotItemIds2, _schemaData->slotItemLevels2, _schemaData->slotItemCount2, _conditionSet))
		{
			break;
		}

		if (false == MakeCondition(_schemaData->slotItemConditionId3, _schemaData->slotItemIds3, _schemaData->slotItemLevels3, _schemaData->slotItemCount3, _conditionSet))
		{
			break;
		}

		if (false == MakeCondition(_schemaData->slotItemConditionId4, _schemaData->slotItemIds4, _schemaData->slotItemLevels4, _schemaData->slotItemCount4, _conditionSet))
		{
			break;
		}

		if (false == MakeCondition(_schemaData->slotItemConditionId5, _schemaData->slotItemIds5, _schemaData->slotItemLevels5, _schemaData->slotItemCount5, _conditionSet))
		{
			break;
		}

	} while (0);*/

	if (nullptr == _schemaData)
	{
		GSLOG(Error, TEXT("nullptr == _schemaData"));
		return;
	}

	const UScriptStruct* Struct = TBaseStructure<FGsSchemaItemCollection>().Get();
	if (nullptr == Struct)
	{
		GSLOG(Error, TEXT("nullptr == Struct"));
		return;
	}

	const void* ContainerPtr = reinterpret_cast<const void*>(_schemaData);

	int32 idx = 0;
	while (true)
	{
		static const FString SLOT_ITEM_CONDITION_ID_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemConditionId0)).LeftChop(1);
		static const FString SLOT_COST_ITEM_IDS_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemIds0)).LeftChop(1);
		static const FString SLOT_COST_ITEM_LEVELS_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemLevels0)).LeftChop(1);
		static const FString SLOT_COST_ITEM_COUNT = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemCount0)).LeftChop(1);

		FString idName = SLOT_ITEM_CONDITION_ID_NAME;		
		FString materialIdName = SLOT_COST_ITEM_IDS_NAME;
		FString materialLvName = SLOT_COST_ITEM_LEVELS_NAME;
		FString materialCount = SLOT_COST_ITEM_COUNT;

		idName.AppendInt(idx);
		materialIdName.AppendInt(idx);
		materialLvName.AppendInt(idx);
		materialCount.AppendInt(idx);

		const FProperty* idProperty = Struct->FindPropertyByName(*idName);
		const FProperty* materialIdProperty = Struct->FindPropertyByName(*materialIdName);
		const FProperty* materialLvProperty = Struct->FindPropertyByName(*materialLvName);
		const FProperty* materialLvCountProperty = Struct->FindPropertyByName(*materialCount);
		if (nullptr == idProperty || nullptr == materialIdProperty || nullptr == materialLvProperty || nullptr == materialLvCountProperty)
		{
			break;
		}

		CollectionConditionId conditionId = INVALID_COLLECTION_CONDITION_ID;
		const CollectionConditionId* findIdPtr = idProperty->ContainerPtrToValuePtr<CollectionConditionId>(ContainerPtr);
		if (nullptr != findIdPtr)
		{
			conditionId = *findIdPtr;
		}

		if (INVALID_COLLECTION_CONDITION_ID >= conditionId)
		{	// 2023/1/5 PKT - 더이상 읽지 않는다.
			break;
		}

		TArray<ItemId> materialIds;
		FScriptArrayHelper materialIdArray(CastField<FArrayProperty>(materialIdProperty), materialIdProperty->ContainerPtrToValuePtr<void>(ContainerPtr));
		for (int32 i = 0; i < materialIdArray.Num(); ++i)
		{
			const ItemId* materialIdPtr = reinterpret_cast<const ItemId*>(materialIdArray.GetRawPtr(i));
			if (nullptr != materialIdPtr)
			{
				materialIds.Add(*materialIdPtr);
			}
		}

		TArray<Level> materialLvs;
		FScriptArrayHelper materialLvArray(CastField<FArrayProperty>(materialLvProperty), materialLvProperty->ContainerPtrToValuePtr<void>(ContainerPtr));
		for (int32 i = 0; i < materialLvArray.Num(); ++i)
		{
			const Level* materialLvPtr = reinterpret_cast<const Level*>(materialLvArray.GetRawPtr(i));
			if (nullptr != materialLvPtr)
			{
				materialLvs.Add(*materialLvPtr);
			}
		}

		int32 amount = 1;
		const int32* findCountPtr = materialLvCountProperty->ContainerPtrToValuePtr<int32>(ContainerPtr);
		if (nullptr != findCountPtr)
		{
			amount = *findCountPtr;
		}

		if (amount <= 0)
		{
			GSLOG(Warning, TEXT("slotItemCount == 0"));
		}

		TArray<FGsItemCollectionCondition::CollectMaterialData> materialDataSet;
		for (int32 i = 0; i < FMath::Min(materialIds.Num(), materialLvs.Num()); ++i)
		{
			materialDataSet.Emplace(FGsItemCollectionCondition::CollectMaterialData(materialIds[i], materialLvs[i]));
		}

		if (0 < materialDataSet.Num())
		{
			FGsItemCollectionCondition newCondition(conditionId, materialDataSet, amount);
			_conditionSet.Emplace(newCondition);
		}

		++idx;
	}
}

void FGsItemCollectionField::BuildValidDateRange(FDateRange& OutValidDateRange)
{
	OutValidDateRange = FDateRange(0, 0);

	_haveValidDateRange = false;

	/**
	 * 서버 기준시 그대로 기간을 잡는다. 출력 할때만 클라 기준 시로 변경!!
	 */
	do
	{
		if (nullptr == _schemaData)
		{
			GSLOG(Error, TEXT("nullptr == _schemaData"));
			break;
		}

		if (_schemaData->startDateTime.IsEmpty() || _schemaData->endDateTime.IsEmpty())
		{
			// 2023/1/4 PKT - 유효 기간이 설정되어 있지 않다.
			break;
		}

		FDateTime beginDate;
		if (false == FDateTime::Parse(_schemaData->startDateTime, beginDate))
		{
			GSLOG(Error, TEXT("false == FDateTime::Parse(_schemaData->startDateTime, LowerDateTimeUtc)[%s]"), *_schemaData->startDateTime);
			break;
		}
	
		FDateTime endDate;
		if (false == FDateTime::Parse(_schemaData->endDateTime, endDate))
		{
			GSLOG(Error, TEXT("false == FDateTime::Parse_schemaData->endDateTime, UpperDateTimeUtc), InEndData:%s"), *_schemaData->endDateTime);
			break;
		}

		OutValidDateRange = FDateRange(beginDate, endDate);

		_haveValidDateRange = true;

	} while (0);
}

const FText& FGsItemCollectionField::ValidDateRangeText() const
{
	if (_haveValidDateRange && _validDateRangeText.IsEmpty())
	{
		// 2022/12/28 PKT - 서버 시간
		const FDateTime beginServerDate = _validDateRange.GetLowerBoundValue().GetTicks();
		const FDateTime endServerDate = _validDateRange.GetUpperBoundValue().GetTicks();

		// 2022/12/28 PKT - 서버 -> 클라 시간
		FDateTime beginDate = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(beginServerDate);
		FDateTime endDate = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(endServerDate);

		FString beginDateText;
		FString endDateText;
		FGsTimeStringHelper::GetTimeStringNYMDHM(beginDate, beginDateText);
		FGsTimeStringHelper::GetTimeStringNYMDHM(endDate, endDateText);

		_validDateRangeText = FText::FromString(FString::Format(TEXT("{0} ~ {1}"), { *beginDateText, *endDateText }));
	}

	return _validDateRangeText;
}

const FText FGsItemCollectionField::HaveLimitTimeText() const
{
	FText outText;

	if (false == IsHaveLimitTime())
	{
		outText = FText::GetEmpty();
	}
	else
	{
		if (ECollectionSortOrder::Completed != SortOrder())
		{
			// 2023/11/16 PKT - 아이템 등록 완료 후 {0} 동안 활성화 - 일/시/분
			FString strTime;
			FGsTimeStringHelper::GetTimeString(FTimespan(_schemaData->durationTime* ETimespan::TicksPerMinute), strTime);

			FText format;
			FText::FindText(TEXT("ItemCollectionMainText"), TEXT("Collection_AvailablePeriod"), format);

			outText = FText::Format(format, { FText::FromString(*strTime) });
		}
		else
		{
			// 2023/11/16 PKT - {0} 까지 활성화
			FString strTime;
			FGsTimeStringHelper::GetTimeStringNYMDHM(_endDate, strTime);

			FText format;
			FText::FindText(TEXT("ItemCollectionMainText"), TEXT("Collection_ExpiredPeriod"), format);

			outText = FText::Format(format, {FText::FromString(*strTime)});
		}
	}

	return outText;
}