#include "GsCraftTab.h"

#include "Currency/GsCostPackage.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftTab.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRecipe.h"
#include "GsCraftTarget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

FGsCraftTab::FGsCraftTab(const FGsSchemaItemCraftTab* InTableData)
	: _tableData(InTableData)
	, _craftTabId((nullptr != _tableData) ? _tableData->itemCraftTabId : INVALID_CRAFT_TAB_ID)
	, _hasCraftTargetIdIndex((nullptr != _tableData) && (_tableData->specialTab == EGsCraftSpecialTab::Favorite))
{
	if (_hasCraftTargetIdIndex)
	{
		_craftTargetIdSet = MakeUnique<TSet<CraftId>>();
	}
}

bool FGsCraftTab::CanDisplay() const
{
	// 포함된 제작 대상이 하나 이상 CanShow 일 경우
	const TArray<const FGsCraftTarget*>& CraftTargets = GetCraftTargets();
	for (const FGsCraftTarget* CraftTarget : CraftTargets)
	{
		check(!!CraftTarget);

		if (CraftTarget->CanDisplay())
		{
			return true;
		}
	}

	// 자식 탭 중 출력해야 할 탭이 있을 경우
	for (const FGsCraftTab* CraftTab : _childTabs)
	{
		check(!!CraftTab);

		if (CraftTab->CanDisplay())
		{
			return true;
		}
	}

	// root tab
	if (nullptr == _parentCraftTab)
	{
		return true;
	}

	check(!!_tableData);
	return _tableData->specialTab == EGsCraftSpecialTab::Favorite;
}

const FText FGsCraftTab::GetTabTitle() const
{
	if (nullptr == _tableData)
	{
		GSLOG(Warning, TEXT("nullptr == _tableData, _craftTabId:%d"), _craftTabId);
		return FText();
	}

	return _tableData->displayText;
}

const FGsCraftTab* FGsCraftTab::GetParentTab() const
{
	return _parentCraftTab;
}

void FGsCraftTab::SetParentTab(FGsCraftTab* InCraftTab)
{
	_parentCraftTab = InCraftTab;
}

void FGsCraftTab::AddChildTab(const FGsCraftTab* InChildCraftTab)
{
	check(InChildCraftTab);

	_childTabs.AddUnique(InChildCraftTab);
}

bool FGsCraftTab::HasChildTab() const
{
	return 0 < _childTabs.Num();
}

TArray<const FGsCraftTab*> FGsCraftTab::MakeDisplaybleChildTabs() const
{
	TArray<const FGsCraftTab*> ValidChildTabs;
	for (const FGsCraftTab* CraftTab : _childTabs)
	{
		if (nullptr == CraftTab)
		{
			GSLOG(Error, TEXT("nullptr == CraftTab, _craftTabId:%d"), _craftTabId);
			continue;
		}

		if (!CraftTab->CanDisplay())
		{
			continue;
		}

		ValidChildTabs.Add(CraftTab);
	}

	ValidChildTabs.StableSort([](const FGsCraftTab& InA, const FGsCraftTab& InB) -> bool
	{
		check(!!InA._tableData);
		check(!!InB._tableData);

		return InA._tableData->displayOrder < InB._tableData->displayOrder;
	});

	return ValidChildTabs; //nrvo
}

const FGsCraftTab* FGsCraftTab::FindDefaultChildTab() const
{
	const FGsCraftTab* AllTab = nullptr;
	const FGsCraftTab* FirstDisplayTab = nullptr;

	const TArray<const FGsCraftTab*> DisplaybleChildTabs = MakeDisplaybleChildTabs();
	for (const FGsCraftTab* CraftTab : DisplaybleChildTabs)
	{
		if (nullptr == CraftTab)
		{
			GSLOG(Error, TEXT("nullptr == CraftTab, _craftTabId:%d"), _craftTabId);
			continue;
		}

		if (!CraftTab->CanDisplay())
		{
			continue;
		}

		if (nullptr == AllTab && CraftTab->_tableData->specialTab == EGsCraftSpecialTab::All)
		{
			AllTab = CraftTab;
		}
		else if (nullptr == FirstDisplayTab)
		{
			FirstDisplayTab = CraftTab;
		}

		if (nullptr != AllTab && nullptr != FirstDisplayTab)
		{
			break;
		}
	}

	//GSLOG(Error, TEXT("has no displayable child tab. _craftTabId:%d"), _craftTabId);
	return (nullptr != AllTab) ? AllTab : FirstDisplayTab;
}

bool FGsCraftTab::HasCraftTarget(const CraftId InCraftTargetId) const
{
	check(INVALID_CRAFT_ID != InCraftTargetId);

	if (!_hasCraftTargetIdIndex)
	{
		// 인덱싱 시스템을 활성화하지 않아도 정상 동작하지만, 최적화를 위해 활성화를 추천함.
		GSLOG(Warning, TEXT("!_hasCraftTargetIdIndex, 인덱싱이 필요하면 인덱싱 시스템을 활성화하도록 수정하세요."));

		for (const FGsCraftTarget* CraftTarget : _craftTargets)
		{
			if (CraftTarget->_craftTargetId == InCraftTargetId)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return _craftTargetIdSet->Contains(InCraftTargetId);
	}
}

int32 FGsCraftTab::GetCraftTargetCount() const
{
	return _craftTargets.Num();
}

void FGsCraftTab::ClearCraftTargets()
{
	_craftTargets.Empty();
}

void FGsCraftTab::AddCraftTarget(const FGsCraftTarget* InCraftTarget)
{
	check(nullptr != InCraftTarget);

	_craftTargets.AddUnique(InCraftTarget);

	if (_hasCraftTargetIdIndex)
	{
		_craftTargetIdSet->Add(InCraftTarget->_craftTargetId);
	}
}

void FGsCraftTab::InsertCraftTarget(const int32 InIndex, const FGsCraftTarget* InCraftTarget)
{
	check(nullptr != InCraftTarget);

	if (!_craftTargets.Contains(InCraftTarget))
	{
		_craftTargets.Insert(InCraftTarget, InIndex);
	}

	if (_hasCraftTargetIdIndex)
	{
		_craftTargetIdSet->Add(InCraftTarget->_craftTargetId);
	}
}

void FGsCraftTab::RemoveCraftTarget(const CraftId InCraftTargetId)
{
	check(INVALID_CRAFT_ID != InCraftTargetId);

	int32 RemovingIndex = -1;
	for (int32 i = _craftTargets.Num() - 1; 0 <= i; --i)
	{
		if (_craftTargets[i]->_craftTargetId == InCraftTargetId)
		{
			RemovingIndex = i;
			break;
		}
	}

	if (0 <= RemovingIndex)
	{
		_craftTargets.RemoveAt(RemovingIndex);
	}

	if (_hasCraftTargetIdIndex)
	{
		_craftTargetIdSet->Remove(InCraftTargetId);
	}
}

void FGsCraftTab::MakeDisplaybleCraftTargets(OUT TArray<const FGsCraftTarget*>& OutCraftTargets, const bool InImmediatelyCraftFirst) const
{
	const TArray<const FGsCraftTarget*>& CraftTargets = GetCraftTargets();
	for (const FGsCraftTarget* CraftTaret : CraftTargets)
	{
		if (!CraftTaret->CanDisplay())
		{
			continue;
		}

		OutCraftTargets.AddUnique(CraftTaret);
	}

	for (const FGsCraftTab* ChildTab : _childTabs)
	{
		//if (nullptr != ChildTab)
		//{
		//	GSLOG(Error, TEXT("nullptr != CraftTab, _craftTabId:%d"), _craftTabId);
		//	continue;
		//}

		if (nullptr == ChildTab)
		{
			GSLOG(Error, TEXT("nullptr == ChildTab, _craftTabId:%d"), _craftTabId);
			continue;
		}

		if (!ChildTab->CanDisplay())
		{
			continue;
		}

		ChildTab->MakeDisplaybleCraftTargets(OutCraftTargets, false);
	}

	if (InImmediatelyCraftFirst)
	{
		OutCraftTargets.StableSort([](const FGsCraftTarget& InA, const FGsCraftTarget& InB) -> bool
		{
			const bool PlayerHasEnoughA = 0 < InA.GetPlayerCanCraftMaxCount();
			const bool PlayerHasEnoughB = 0 < InB.GetPlayerCanCraftMaxCount();
			if (PlayerHasEnoughA != PlayerHasEnoughB)
			{
				return PlayerHasEnoughA;
			}

			check(InA._tableData);
			check(InB._tableData);

			return InA._tableData->displayOrder < InB._tableData->displayOrder;
		});
	}
}

void FGsCraftTab::InitializeCraftTargets()
{
	if (_initializeCraftTargets)
	{
		return;
	}
	_initializeCraftTargets = true;

	if (nullptr == _parentCraftTab || EGsCraftSpecialTab::All != _tableData->specialTab)
	{
		return;
	}

	// EGsCraftSpecialTab::All 인 경우, 부모와 형제의 모든 CraftTarget 을 입력함
	TArray<const FGsCraftTab*> CraftTabs;
	CraftTabs.Add(_parentCraftTab);
	CraftTabs.Append(_parentCraftTab->_childTabs);

	for (const FGsCraftTab* CraftTab : CraftTabs)
	{
		check(!!CraftTab);

		if (CraftTab == this)
		{
			continue;
		}

		const TArray<const FGsCraftTarget*>& CraftTargets = CraftTab->GetCraftTargets();
		for (const FGsCraftTarget* CraftTarget : CraftTargets)
		{
			AddCraftTarget(CraftTarget);
		}
	}
}

const TArray<const FGsCraftTarget*>& FGsCraftTab::GetCraftTargets() const
{
	const_cast<FGsCraftTab*>(this)->InitializeCraftTargets();
	return _craftTargets;
}

CraftTabId FGsCraftTab::MakeCraftTabId(const FName& InCraftTabIdName)
{
	CraftTabId TabId = INVALID_CRAFT_TAB_ID;
	LexFromString(TabId, *InCraftTabIdName.ToString());
	return TabId;
}
