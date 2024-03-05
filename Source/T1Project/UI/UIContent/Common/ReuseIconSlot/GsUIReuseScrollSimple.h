//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseScroll.h"
//#include "UI/UIControlLib/Define/GsIconDefine.h"
//#include "Management/ScopeGame/GsNpcShopManager.h"
//#include "GsUIReuseScrollSimple.generated.h"
//
///**
// * 
// */
//UCLASS(meta = (DisableNativeTick))
//class T1PROJECT_API UGsUIReuseScrollSimple : public UGsUIReuseScroll
//{
//	GENERATED_BODY()
//	
//
//
//public:
//	void SetDefaultPage();
//	void SetSlotAutoFocusing(const int32 InSlotIndex);
//
//public:
//	void InvalidateAll();
//	void InvalidateAll(TArray<FGsNpcShopQuickSlotItemInfo*> InNpcShopSlotInfo);
//
//private:
//	virtual void InvalidateAllSlot() override;
//	virtual void InvalidateSlotList(TArray<uint8>& InSlotIndexList) override;
//	virtual void InvalidateSwitcher() override;
//
//protected:
//	virtual void NativeOnInitialized() override;
//	virtual void NativeConstruct() override;
//	virtual void NativeDestruct() override;
//
//};
