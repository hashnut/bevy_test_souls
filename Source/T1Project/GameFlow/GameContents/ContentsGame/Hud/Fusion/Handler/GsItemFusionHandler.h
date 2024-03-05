#pragma once

#include "CoreMinimal.h"
#include "../Item/GsItem.h"
#include "IDelegateInstance.h"
#include "../Message/GsMessageItem.h"
#include "../Message/GsMessageContents.h"
#include "../Message/GsMessageGameObject.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../UI/UIContent/Popup/ItemFusion/GsUIPopupItemFusion.h"


class T1PROJECT_API FGsItemFusionHandler : public IGsEventProgressInterface
{
private:	
	ItemDBId _srcItemDBId = INVALID_ITEM_DB_ID;
	ItemDBId _destItemDBId = INVALID_ITEM_DB_ID;

	ItemDBId _refainItemDBId = INVALID_ITEM_DB_ID;

	bool _messageHolder = false;

private:	
	// 2023/9/25 PKT - Static Bind
	TPair<MessageContentsFusion, FDelegateHandle> _uiStaticDelegate;
	
	// 2023/9/25 PKT - Dynamic Bind
	TArray<TPair<MessageContentsFusion, FDelegateHandle>> _uiDynamicDelegates;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDynamicDelegates;		// 2023/9/25 PKT - Item Delegate
	TArray<TPair<MessageGameObject, FDelegateHandle>> _listGameObjectDynamicDelegates;	// 2023/9/25 PKT - GameObject Delegate

private:
	void ClearData();

	TWeakObjectPtr<UGsUIPopupItemFusion> GetPopup(bool InIsOpen = false);
	void InvalidateUI();

	/**
	 * 2023/9/25 PKT -
	 * Event Progress Interface
	 */
public:
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

public:
	FGsItemFusionHandler() = default;
	virtual ~FGsItemFusionHandler();

	void Init();
	void Close();
	void OnReconection();

private:
	// 2023/9/25 PKT - 강화창이 활성/비활성화 상태일때 동적으로 연결
	void BindStaticMessage();
	void UnBindStaticMessage();

	// 2023/9/25 PKT - 강화창이 활성/비활성화 상태일때 동적으로 연결
	void BindDynamicMessage();
	void UnBindDynamicMessage();

	const struct FGsSchemaItemEquipFuse* GetSchemaItemEquipFuse(const ItemId InItemId) const;
	const struct FGsSchemaItemEquipFuse* GetSchemaItemEquipFuse(const TWeakPtr<FGsItem> InItem) const;
	const struct FGsSchemaItemEquipFuse* GetSchemaItemEquipFuse(const ItemDBId InItemDBId) const;

public:
	// 2023/9/25 PKT - 합성 아이템 공통 규칙 적용(Src / Dest, 장비/참)
	enum class ECheckValid {Valid, NonFusionInvalid, LevelInvalid, EquipInvalid, PresetInvalid, LockInvalid, ContentsUnLockInvalid};
	TPair<ECheckValid,int32> IsTargetItemSuitable(const TWeakPtr<FGsItem> InItem) const;

	// 2023/10/11 PKT - 합성 시도 체크 사항
	enum class ECheckTry { Valid, InvalidRegist, InvalidRefine, invalidCurrency };
	ECheckTry IsCheckTryFusion() const;

	ItemDBId GetSrcItemDBId() const { return _srcItemDBId; }
	ItemDBId GetDestItemDBId() const { return _destItemDBId; }
	ItemDBId GetRefinedItemDBId() const { return _refainItemDBId; }
	
	const TArray<class FGsItemStat*> GetItemRefineStatSet(ItemDBId InItemDBId) const;

	// 2023/10/5 PKT - 합성 비용
	CurrencyPair GetCost() const;

	// 2023/9/26 PKT - SrcItem 으로 적합한지 검증
	bool IsSuitableForSrcItem(const TWeakPtr<FGsItem> InItem, bool InIsPrintMessage = false) const;
	bool IsSuitableForSrcItem(const ItemDBId InItemDBId, bool InIsPrintMessage = false) const;

	// 2023/9/26 PKT - DestItem 으로 적합한지 검증
	bool IsSuitableForDestItem(const TWeakPtr<FGsItem> InItem) const;
	bool IsSuitableForDestItem(const ItemDBId InItemDBId) const;
	
	const FGsItem PreviewFusionResult() const;
 
public:
	/**
	 * Local Player Delegate
	 */
	void OnLocalPlayerStateChange(const struct IGsMessageParam* InParam);
	
	/**
	 * Item Delegate
	 */
	void OnAddItemList(struct FGsItemMessageParamBase& InParam);
	void OnUpdateItem(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItem(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItemList(struct FGsItemMessageParamBase& InParam);
	void OnLocalPlayerUseItemLock(struct FGsItemMessageParamBase& InParam);
	
	/**
	 * Fusion Delegate
	 */
	void OnOpenItemFusionProc(const struct IGsMessageParam* InParam);
	void OnTryItemFusionResult(const struct IGsMessageParam* InParam);

public:
	void TryFusion();
	void PushMaterialItem(ItemDBId InItemDBId);
	void PopMaterialItem(ItemDBId InItemDBId);
	void SelectedRefain(ItemDBId InItemDBId);
	void CleanView();
	void CloseUI();
};