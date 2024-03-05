// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "MultiLevelRank/GsMultiLevelRankDefine.h"
#include "GsUIMultiRankRewardData.generated.h"


class UImage;
class UWidget;
class UTextBlock;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconItem;

/**
* file		GsUIMultiRankRewardData.h
* @brief	다단계 랭크 시스템 디테일 Panel에서 보상 정보 UI
* @author	PKT
* @date		2021/06/22
* */
UCLASS()
class T1PROJECT_API UGsUIMultiRankRewardData : public UUserWidget
{
	GENERATED_BODY()
	
	/************************************************************************/
	/* // 2021/06/22 PKT - BP bind                                          */
	/************************************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _emptyStateBG;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _toggleOnEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textName;

	UPROPERTY();
	UGsDynamicIconSlotHelper* _slotHelper;
	

	/************************************************************************/
	/* 2021/06/22 PKT - 멤버 변수                                           */
	/************************************************************************/
	FGsRefreshRewardBuffer _rewardData;


	/************************************************************************/
	/* // 2021/06/22 PKT - Delegate                                         */
	/************************************************************************/
protected:
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon);


	/************************************************************************/
	/* 2021/06/21 PKT - Construct / Destruct                                */
	/************************************************************************/
public:
	virtual void BeginDestroy() override;
public:
	virtual void NativeOnInitialized() override;
	

	/************************************************************************/
	/* // 2021/06/22 PKT - invaildateData                                   */
	/************************************************************************/
	void InvalidateData(const FGsRefreshRewardBuffer& InRewardData);

private:
	void OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon);
};
