// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseItemIconSlotBase.h"
#include "GsUIReuseItemIconSlotSimple.generated.h"

class UGsUIIconBase;
/**
 * 기본적인 퀵슬롯에서 사용되는 class
 * ex) npc상점 같은곳에서 사용되는 용도
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIReuseItemIconSlotSimple : public UGsUIReuseItemIconSlotBase
{
	GENERATED_BODY()

protected:
	virtual void OnShow() override;
	virtual void OnHide() override;

protected:
	virtual void NativeOnInitialized() override;

protected:
	virtual void OnClickIconSlot(class UGsUIIconItem& InIcon) override;

public:
	virtual void SetSlotItemIcon(const uint64 InItemDBID, const int32 InItemTID, const int64 InAmount, const uint8 InSlotIndex = 0);
	
};
