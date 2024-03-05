// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListBase.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Reward/GsRewardIconDataWrapper.h"
#include "GsUIRewardIconListDefault.generated.h"

struct FGsRewardUnboxingData;
class UGsUIIconBase;

/**
 * 보상 아이콘 리스트 기본. 갯수가 달라도 상관 없음
 */
UCLASS()
class T1PROJECT_API UGsUIRewardIconListDefault : public UGsUIRewardIconListBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	// 동적 아이콘 슬롯 관리
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _listHelper;

protected:
	// 아이콘 슬롯 토글 기능 관리
	FGsToggleGroup _toggleGroup;
	bool _bEnableToggleGroup = false;

	TArray<FGsRewardIconDataWrapper> _dataList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void SetData(const FGsRewardUnboxingData* InData) override;

	virtual void SetEnableToggleGroup(bool bInEnable) override { _bEnableToggleGroup = bInEnable; }
	virtual ItemBagIndex GetSelectedIndex() const override;

protected:
	void InvalidateIconList();

	// 아이콘 선택 가능여부
	bool GetCanSelectIcon() const;

protected:
	// 아이콘 생성
	UFUNCTION()
	void OnCreateIcon(UGsUIIconBase* InIcon);

	// 아이콘 갱신
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

	// 아이콘 클릭 시
	UFUNCTION()
	void OnClickRewardIcon(int32 InIndex);
};
