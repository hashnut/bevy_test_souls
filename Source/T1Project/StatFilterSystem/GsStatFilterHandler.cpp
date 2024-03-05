#include "GsStatFilterHandler.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../UI/UIContent/Tray/GsUITrayCommonStatFilterInCategory.h"

#include "../GameObject/Stat/GsStatHelper.h"

#include "GsTable.h"
#include "DataSchema/Stat/GsSchemaCommonStatFilterGroup.h"
#include "DataSchema/Stat/GsSchemaCommonStatFilterCategory.h"

#include "T1Project.h"


FGsStatFilterHandler::~FGsStatFilterHandler()
{
	Reset();
}

void FGsStatFilterHandler::Init()
{
	Reset();
}

void FGsStatFilterHandler::Close()
{
	Reset();
}

void FGsStatFilterHandler::OnReconection()
{
	Reset();
}

void FGsStatFilterHandler::OpenWidget(const TArray<StatType>& InContentsAllStatSet, const TArray<StatType>& InSelectedStatSet, TFunction<void(TArray<StatType>&)> InCallback)
{
	if (0 >= _categoryItemSet.Num())
	{	// 2024/1/17 PKT - 처음 들어 왔거나, 어떠한 사유로 초기화 되었다면 카테고리 정보를 만들어 준다.
		MakeCategoryItems();
	}

	TWeakObjectPtr<UGsUITrayCommonStatFilterInCategory> widget
		= Cast<UGsUITrayCommonStatFilterInCategory>(GUI()->OpenAndGetWidget(TEXT("TrayCommonStatFilterInCateogry")));

	if (widget.IsValid())
	{
		widget->SetData(InContentsAllStatSet, InSelectedStatSet, InCallback);
	}
}

void FGsStatFilterHandler::Reset()
{
	_statFilterItemSet.Empty();
	_categoryItemSet.Empty();	
}

void FGsStatFilterHandler::MakeCategoryItems()
{
	Reset();

	const UGsTableCommonStatFilterCategory* table = Cast<UGsTableCommonStatFilterCategory>(FGsSchemaCommonStatFilterCategory::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("nullptr == table"));
		return;
	}

	TArray<const FGsSchemaCommonStatFilterCategory*> categoryDataItems;
	if (false == table->GetAllRows(categoryDataItems))
	{
		GSLOG(Error, TEXT("false == table->GetAllRows(categoryDataItems)"));
		return;
	}

	for (const FGsSchemaCommonStatFilterCategory* categoryItem : categoryDataItems)
	{
		FGsStatFilterCategoryItem item;

		item._order = categoryItem->id;
		item._categoryName = categoryItem->categoryName;
		item._statSet.Empty();

		for (const FGsSchemaCommonStatFilterGroupRow& row : categoryItem->statFilterSet)
		{
			const FGsSchemaCommonStatFilterGroup* statGroup = row.GetRow();
			if (nullptr != statGroup)
			{
				for (const FGsStatTypeNameSelector& stat : statGroup->statGroupSet)
				{
					StatType statType = stat.GetStatType();
					if (false == item._statSet.Contains(statType))
					{
						item._statSet.Emplace(statType);
					}
				}
			}			
		}
		// 2024/1/16 PKT - Add..
		_categoryItemSet.Emplace(item);
	}

	// 2024/1/16 PKT - 카테고리 정렬
	_categoryItemSet.Sort([](const FGsStatFilterCategoryItem& lhr, const FGsStatFilterCategoryItem& rhr)
		{
			return lhr._order < rhr._order;
		}
	);

	for (int32 idx = 0; idx < _categoryItemSet.Num(); ++idx)
	{
		FGsStatFilterCategoryItem& item = _categoryItemSet[idx];

		// 2024/1/16 PKT - 카테고리 내 Stat 정렬
		item._statSet.Sort([](const StatType lhr, const StatType rhr)
			{
				return lhr < rhr;
			}
		);
	}
}

int32 FGsStatFilterHandler::CategoryMaxCount() const
{
	return _categoryItemSet.Num();
}

const FGsStatFilterCategoryItem* FGsStatFilterHandler::FindCatgegoryItem(int32 InIndex) const
{
	return _categoryItemSet.IsValidIndex(InIndex) ? &(_categoryItemSet[InIndex]) : nullptr;
}

const FGsStatFilterItem* FGsStatFilterHandler::FindOrAddStatFilterItem(StatType InStatType) const
{
	int32 findIndex = _statFilterItemSet.IndexOfByPredicate([InStatType](const FGsStatFilterItem& InData)
		{
			return InData._type == InStatType;
		}
	);
	
	if (INDEX_NONE >= findIndex)
	{
		FGsStatFilterItem item;
		item._type = InStatType;
		FGsStatHelper::GetDisPlayStatName(InStatType, item._name);
		item._cachedTrimName = item._name.ToString();
		item._cachedTrimName.ReplaceInline(TEXT(" "), TEXT(""));
		
		// 2024/1/17 PKT - 마지막에 추가. FindIndex는 -1해야 자릿수가 맞음.
		findIndex = _statFilterItemSet.Emplace(item);
	}

	return _statFilterItemSet.IsValidIndex(findIndex) ? &(_statFilterItemSet[findIndex]) : nullptr;
}