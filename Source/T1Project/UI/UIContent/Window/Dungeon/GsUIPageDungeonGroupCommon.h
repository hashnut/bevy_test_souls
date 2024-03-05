// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsSchemaEnums.h"
#include "GsUIPageDungeonGroupBase.h"
#include "../Dungeon/GsDungeonUICaptureData.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "../UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "../UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Classes/GsPoolUObject.h"
#include "GsUIPageDungeonGroupCommon.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPageDungeonGroupCommon : public UGsUIPageDungeonGroupBase
{
	GENERATED_BODY()
	
protected:
	// 던전그룹 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelectorEnv;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGoWorldMap;

	// 던전그룹 설명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textDesc;

	// 던전 대표 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgDungeonGroup;

	// 슬롯 class
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBoxDungeonSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetDungeonSetClass;
		
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelperDungeonSet;
	

	// 던전 기본 제공 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBaseTime;

	// 던전 추가 충전 시간 Switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherChargeTime;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	// 던전 추가 충전 시간 TextBlock
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textChargeTime;

	// 초기화 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textResetTime;
	
	// 충전버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCharge;

	// 2022/08/18 PKT - 충전 정보 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _panelChargeRect;

	// 2022/08/18 PKT - 충전 아이템 개수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textChargeItemCount;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _listViewRewardSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textLevelLimit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencyButton* _btnTryEntered;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnResetTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelEliteRewardZone;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textEliteRewardTip;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnEliteRewardTooltip;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipDesc* _tooltipResetTimeInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipDesc* _tooltipEliteRewardInfo;	

	UPROPERTY()
	UGsPoolUObject* _rewardListViewEntryPool;
protected:
	FGsToggleGroup _toggleGroupDungeonSet;

	// 2023/5/10 PKT - 가끔 가비지를 통해 포인터가 날아가는 것 같아서 'TWeakObjectPtr' -> UPROPERTY 로 변경
	TWeakObjectPtr<UGsUIIconItemInventory> _iconChargeItem;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void InvalidateDungeonDetailInfo();

protected:
	UFUNCTION()
	void OnRefreshDungeonSetEntry(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnSelectedDungeon(int32 InIndex);

	// 2023/2/22 PKT - Party Dungeon Only
	UFUNCTION()
	void OnClickedGoTryToDungeon();

	UFUNCTION()
	void OnClickedCharge();

	UFUNCTION()
	void OnClickedGoWorldMap();

	UFUNCTION()
	void OnClickedEnvironmentIcon(class UGsUIIconAbnormality* InIcon);

	UFUNCTION()
	void OnClickedResetTooltip();

	UFUNCTION()
	void OnClickedEliteRewardTooltip();

	void OnLongPressIconChargeItem(UGsUIIconItem& InIcon);

public:
	virtual void Enter(const class FGsDungeonGroupBase* InData, const FGsDungeonUICaptureData& InCaptureData);
	virtual void Invalidate();
	virtual void Capture(FGsDungeonUICaptureData* OutCaptherData) const;
};
