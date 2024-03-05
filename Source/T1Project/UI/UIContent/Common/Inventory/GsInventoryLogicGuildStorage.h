//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameObject/Define/GsGameObjectDefine.h"
//#include "Message/GsMessageContents.h"
//#include "Shared/Shared/SharedInclude/SharedTypes.h"
//#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
//#include "UI/UIContent/Common/Inventory/GsInventoryLogicBase.h"
//
//
//class UGsUIIconBase;
//class FGsItem;
///**
//* 기사단 창고 인벤토리 처리 클래스
//**/
//class T1PROJECT_API FGsInventoryLogicGuildStorage final : public FGsInventoryLogicBase
//{
//public:
//	using Super = FGsInventoryLogicBase;
//
//protected:
//	TArray<FGsItem*> _dataList;
//
//	ItemAmount _guildOwnedItemCount = 0;
//	ItemAmount _invenTotalSlotCount = 0;
//	uint32 _totalExtendCount = 0;
//
//public:
//	FGsInventoryLogicGuildStorage() = default;
//	virtual ~FGsInventoryLogicGuildStorage() = default;
//
//public:
//	virtual void Clear() override;
//
//public:
//	void OnNativeConstruct();
//	void OnChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount);
//
//public:
//	virtual void OnInvalidateList() override;
//	virtual void OnCreateItemIcon(UGsUIIconBase * InIcon) override;
//	virtual void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon) override;
//	virtual void OnClickItemIcon(UGsUIIconItem & InIcon) override;
//	virtual void OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon) override;
//	virtual void OnClickSlotLock() override;
//	virtual void OnSelectTab(EInventoryTab InType) override;
//
//	virtual int32 GetDataCount() const override;
//	virtual int32 GetDataIndex(ItemDBId InItemDBId) const override;
//	virtual int32 GetEnoughExtendSlotCount() const override;
//	
//	virtual ItemAmount GetInventorySlotCount() const override;
//	virtual ItemAmount GetOwnedItemCount() const override;
//
//	virtual bool CheckRedDotByInvenTab(EInventoryTab InTab) override;
//
//public:
//	FGsItem* GetDataByIndex(int32 InIndex);
//
//protected:
//	void GetItemList(EInventoryTab InTabType);
//
//public:
//	void UpdateSelectedItem();
//	void CheckExpiredItem();
//
//protected:
//	void OnLongPressIcon(UGsUIIconItem& InIcon);
//
//
//};