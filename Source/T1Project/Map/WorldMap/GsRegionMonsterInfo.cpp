#include "GsRegionMonsterInfo.h"

#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"

FGsRegionMonsterItemInfo::FGsRegionMonsterItemInfo(const FGsSchemaItemCommon* In_tbl)
{
	_itemTbl = In_tbl;

	if (In_tbl == nullptr)
	{
		return;
	}

	switch (In_tbl->categoryMain)
	{
	case ItemCategoryMain::WEAPON:
		_itemSortType = EGsRegionItemSecondSortType::WEAPON;
		break;
	case ItemCategoryMain::ARMOR:
		_itemSortType = EGsRegionItemSecondSortType::ARMOR;
		break;
	case ItemCategoryMain::ACCESSORY:
		_itemSortType = EGsRegionItemSecondSortType::ACCESSARY;
		break;
	case ItemCategoryMain::SKILLBOOK:
		_itemSortType = EGsRegionItemSecondSortType::SKILLBOOK;
		break;
	default:
		_itemSortType = EGsRegionItemSecondSortType::ETC;
		break;
	}
}

FGsRegionMonterItemInfoEtc::FGsRegionMonterItemInfoEtc(const FGsSchemaItemCommon* In_tbl) : FGsRegionMonsterItemInfo(In_tbl)
{
	if (In_tbl != nullptr)
	{
		switch (In_tbl->categoryMain)
		{
		case ItemCategoryMain::SKILLBOOK:
			_sortType = EGsRegionEtcItemSecondSortType::SKILLBOOK;
			break;
		case ItemCategoryMain::CRAFT:
			_sortType = EGsRegionEtcItemSecondSortType::CRAFT;
			break;
		case ItemCategoryMain::SCROLL:
			_sortType = EGsRegionEtcItemSecondSortType::SCROLL;
			break;
		case ItemCategoryMain::MONSTER_KNOWLEDGE:
			_sortType = EGsRegionEtcItemSecondSortType::MONSTER_KNOWLEDGE;
			break;
		default:
			_sortType = EGsRegionEtcItemSecondSortType::ETC;
			break;
		}
	}
}

void FGsRegionMonsterItemInfoSet::Finalize()
{

	if (_arrayItem.Num() != 0)
	{
		for (FGsRegionMonsterItemInfo* iter : _arrayItem)
		{
			if (iter != nullptr)
			{
				iter->Finalize();
				delete iter;
			}
		}
		_arrayItem.Empty();
	}
}

// Á¤¸®
void FGsRegionMonsterSameIDInfo::Finalize()
{
	if (_arrayLevel.Num() != 0)
	{
		_arrayLevel.Empty();
	}
}