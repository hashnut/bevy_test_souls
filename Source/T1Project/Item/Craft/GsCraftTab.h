#pragma once

#include "GsCraftTarget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsCraftTarget;
struct FGsSchemaItemCraftTab;

/**
 * 제작탭
 */
class FGsCraftTab final
{

public:
	const FGsSchemaItemCraftTab* _tableData = nullptr;

	const CraftTabId _craftTabId = INVALID_CRAFT_TAB_ID;

	const bool _hasCraftTargetIdIndex = false;

private:
	FGsCraftTab* _parentCraftTab = nullptr;

	bool _initializeCraftTargets = false;

    TArray<const FGsCraftTab*> _childTabs;

    TArray<const FGsCraftTarget*> _craftTargets;

	/**
	 * 포함된 CraftTarget 의 ID 모음.
	 * 즐겨찾기 탭 등 빠른 인덱싱이 필요한 경우에만 유효함
	 */
	TUniquePtr<TSet<CraftId>> _craftTargetIdSet;

public:
    explicit FGsCraftTab(const FGsSchemaItemCraftTab* _tableData);

public:
	bool CanDisplay() const;

    const FText GetTabTitle() const;

	const FGsCraftTab* GetParentTab() const;

	void SetParentTab(FGsCraftTab* InCraftTab);

	/************************************************************************/
	/* child tabs															*/
	/************************************************************************/

public:
	void AddChildTab(const FGsCraftTab* InChildCraftTab);

	bool HasChildTab() const;

	TArray<const FGsCraftTab*> MakeDisplaybleChildTabs() const;

	/**
	 * 자식 탭 중에서 기본 선택이 가능한 탭을 반환함.
	 * EGsCraftSpecialTab::All 인 자식탭이 있다면 해당 탭을 반환하고, 없다면 출력 가능한 모든 탭 중에서 첫번째 탭을 반환함.
	 */
	const FGsCraftTab* FindDefaultChildTab() const;

	/************************************************************************/
	/* craft targets														*/
	/************************************************************************/

public:
	bool HasCraftTarget(const CraftId InCraftTargetId) const;

	int32 GetCraftTargetCount() const;

	void ClearCraftTargets();

	void AddCraftTarget(const FGsCraftTarget* InCraftTarget);

	void InsertCraftTarget(const int32 InIndex, const FGsCraftTarget* InCraftTarget);

	void RemoveCraftTarget(const CraftId InCraftTargetId);

	void MakeDisplaybleCraftTargets(OUT TArray<const FGsCraftTarget*>& OutCraftTargetIds, const bool InImmediatelyCraftFirst) const;

private:
	void InitializeCraftTargets();

	const TArray<const FGsCraftTarget*>& GetCraftTargets() const;

	/************************************************************************/
	/* utils																*/
	/************************************************************************/

private:
	static CraftTabId MakeCraftTabId(const FName& InCraftTabIdName);

};
