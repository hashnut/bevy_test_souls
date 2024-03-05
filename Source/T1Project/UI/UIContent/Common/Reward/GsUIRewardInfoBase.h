// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Reward/GsRewardHelper.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIRewardInfoBase.generated.h"


/**
 * 보상 기본 UI. InRewardId를 입력받아 동작
 * - UGsUIRewardInfoMainBase
 * - UGsUIRewardInfoMainSubBase
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIRewardInfoBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool SetData(RewardId InRewardIdMain, RewardId InRewardIdSub) { return false; }
	virtual bool SetData(RewardId InRewardIdMain, RewardId InRewardIdSub, const TArray<RewardBoxItemIdPair>& InList) { return false; }
	virtual bool SetData(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData) { return false; }

	// 상세창 사용여부, 출력 위치
	virtual void SetDetailPopup(bool bInEnable, bool bInCenterPopup = true) {}

	// 타이틀 바꾸고 싶은 경우에 사용
	virtual void SetTitleText(const FText& InText) {}
	virtual void SetSubTitleText(const FText& InText) {}
	virtual void SetVisibleTitle(bool bIsVisible) {}


	// RewardType::PICK_ONE 관련 기능	
	// 토글 그룹(선택 기능) 사용여부
	virtual void SetEnableToggleGroup(bool bInEnable) {}
	virtual ItemBagIndex GetSelectedIndexMain() const { return INVALID_ITEM_BAG_INDEX; }
	virtual ItemBagIndex GetSelectedIndexSub() const { return INVALID_ITEM_BAG_INDEX; }
	virtual RewardType GetRewardTypeMain() const { return RewardType::MAX; }
	virtual RewardType GetRewardTypeSub() const { return RewardType::MAX; }

	// RewardType::RANDOM_ONE, PICK_REWARD_BOX_LIST 관련 기능
	// 주의: 서버에서 준 랜덤 보상 결정된 인덱스. SetData 이후에 호출 필수
	virtual void SetReceivedIndex(ItemBagIndex InIndex) {}
	virtual bool IsRandomEffect() const { return false; }
};
