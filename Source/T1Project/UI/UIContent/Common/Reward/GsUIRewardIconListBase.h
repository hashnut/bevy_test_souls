// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Reward/GsRewardHelper.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIRewardIconListBase.generated.h"

/**
 * 보상 아이콘 리스트의 기본 인터페이스.
 * - UGsUIRewardIconListDefault, UGsUIRewardIconListRandom
 */
UCLASS()
class T1PROJECT_API UGsUIRewardIconListBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	const FGsRewardUnboxingData* _rewardData = nullptr;
	
	// 상세정보
	bool _bEnableDetailPopup = true;
	bool _bCenterPopup = true;

protected:
	//virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void SetData(const FGsRewardUnboxingData* InData);

	// Default에서만 사용
	virtual void SetEnableToggleGroup(bool bInEnable) {}
	virtual ItemBagIndex GetSelectedIndex() const { return INVALID_ITEM_BAG_INDEX; }
	
	// Random에서만 사용
	virtual void SetReceivedIndex(ItemBagIndex InIndex) {}

public:
	void SetDetailPopup(bool bInEnable, bool bInCenterPopup = true)
	{
		_bEnableDetailPopup = bInEnable;
		_bCenterPopup = bInCenterPopup;
	}

protected:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);
};
