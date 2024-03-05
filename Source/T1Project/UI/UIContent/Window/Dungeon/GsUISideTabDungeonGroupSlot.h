// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUISideTabDungeonGroupSlot.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISideTabDungeonGroupSlot : public UUserWidget, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	// 던전 대표 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgDungeonGroup;

	// 환경 효과 아이콘
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelectorEnv;

	// 던전그룹 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelTimeInfo;
	
	// 던전 기본 제공 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBaseTime;

	// 던전 추가 충전 시간 Switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherChargeTime;

	// 던전 추가 충전 시간 TextBlock
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textChargeTime;

	// 초기화 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textResetTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _contentsBindType;

	// 안전구역, 일반구역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherPk;

	// 2023/3/24 PKT - Event
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _markEvent;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _selectedEdgeImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _canvasPanelLockContents;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textLockReason;

	const class FGsDungeonGroupBase* _data = nullptr;

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, const class FGsDungeonGroupBase*);
	FOnClickSlot _OnClickedSlotDelegate;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;

private:
	void DrawPKMark(const class FGsDungeonGroupBase* InData);
	void SetTime(const class FGsDungeonGroupBase* InDungeonGroup);

protected:
	// 슬롯 클릭
	UFUNCTION()
	void OnClickSlot();

public:
	void SetData(const class FGsDungeonGroupBase* InDungeonGroup);
	void Invalidate();

	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	const FGsDungeonGroupBase* GetData() const;
};
