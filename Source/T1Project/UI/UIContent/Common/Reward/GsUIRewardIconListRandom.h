// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListBase.h"
#include "Reward/GsRewardHelper.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Classes/GsPoolUObject.h"
#include "Reward/GsRewardIconDataWrapper.h"
#include "GsUIRewardIconListRandom.generated.h"

class UListView;
class UCurveFloat;
class UPanelWidget;
class UGsRewardIconListRandomListItem;
struct FGsRewardUnboxingData;

/**
 * 랜덤 보상. 룰렛 연출
 */
UCLASS()
class T1PROJECT_API UGsUIRewardIconListRandom : public UGsUIRewardIconListBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsUIRewardIconListRandomListItem))
	UListView* _listView;
	UPROPERTY(EditAnywhere, Category = "GsUI")
	UCurveFloat* _curve;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEffect;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	int32 _repeatCount = 6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _offsetX = 0.05f;

protected:
	UPROPERTY()
	UGsPoolUObject* _poolItem;

protected:
	TArray<UGsRewardIconListRandomListItem*> _itemList;
	ItemBagIndex _selectedIndex = INVALID_ITEM_BAG_INDEX;
	const FGsRewardUnboxingData* _rewardData = nullptr;
	TArray<FGsRewardIconDataWrapper> _dataList;
	
	bool _bIsPlayingAnimation = false;
	float _currTime = 0.f;
	float _totalDist = 0.f;
	float _maxTime = 0.f;

	uint64 _soundId = 0;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void SetData(const FGsRewardUnboxingData* InData) override;

	// Random에서만 사용. 주의: SetData 보다 먼저 정보를 넣을 것
	virtual void SetReceivedIndex(ItemBagIndex InIndex) override;

protected:
	void SetItemList();
	void InvalidateList();
	void StartScrollAnimation();
	void EndScrollAnimation();

protected:
	UFUNCTION()
	void OnScrollListView(float InOffsetInItems, float InDistanceRemaining);

	// 스크롤 시작 시
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartScrollAnimation();
	// 스크롤 종료 시
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndScrollAnimation();
};
