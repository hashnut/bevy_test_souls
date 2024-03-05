#pragma once


#include "CoreMinimal.h"
#include "Item/Collection/GsItemCollectionSlotState.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemCollectionSlot.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicBase.h"
#include "Message/GsMessageContents.h"

class UGsUIIconBase;
class UGsUIIconItem;
class FGsItem;

/**
* file		GsInventoryLogicItemCollection.h
* @brief	Item Collection 전용으로 사용 되어질 인벤토리
* @author	PKT
* @date		2021/08/06, 2021/10/12(Guild Item Collection 리뉴얼)
**/
class T1PROJECT_API FGsInventoryLogicItemCollection final : public FGsInventoryLogicBase
{
public:
	using Super = FGsInventoryLogicBase;

protected:
	TArray<const FGsItem*> _dataList;

	TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>> _materialSet;

public:
	FGsInventoryLogicItemCollection() = default;
	virtual ~FGsInventoryLogicItemCollection() = default;

public:
	virtual void Clear() override;
	virtual void Tick(int32 InDeltaSeconds) override;

	virtual void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase * InIcon) override;
	virtual void OnSelectItemIcon(UGsUIIconItem * InPrevIcon, UGsUIIconItem * InCurrIcon) override;

	// 2021/10/15 PKT - 데이터 갯수
	virtual int32 GetDataCount() const override;

	// 2021/10/15 PKT - 해당 ItemDBId의 인덱스 얻기
	virtual int32 GetDataIndex(ItemDBId InItemDBId) const override;

	// 2021/10/15 PKT - Index로 Item 정보 얻기
	const FGsItem* GetItemByIndex(int32 InIndex) const;

	// 2021/10/15 PKT - Index로 Item 정보 얻기
	const FGsItem* GetItemByDBId(ItemDBId InItemDBId) const;

	// 2021/10/15 PKT - Item TId로 관련 아이템이 몇개 있는지 찾기( Key : Count, Value : Index )
	void FindItemSet(const TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>>&InMaterialSet
		, TArray<ItemDBId>* OutFindItemSet) const;

	/************************************************************************/
	/* Invalidate                                                           */
	/************************************************************************/
public:
	virtual void InvalidateList(const TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>>& InMaterialSet);

	virtual void OnInvalidateList();

	virtual void InvalidateSelected(ItemDBId InItemDBId);

	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
protected:	
	void GetItemList(EInventoryTab InTabType, const TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>>& InMaterialSet);
};