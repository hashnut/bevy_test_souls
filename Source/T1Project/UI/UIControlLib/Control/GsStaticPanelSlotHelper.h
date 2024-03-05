// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

/**
 * FGsDynamicPanelSlotHelper
 * : 동적으로 생성하지 않고, UI에 만들어 둔 리스트를 사용. 고정리스트에서 활용.
 */
class T1PROJECT_API FGsStaticPanelSlotHelper
{
public:
	DECLARE_DELEGATE_TwoParams(FOnDynamicPanelSlotRefresh, int32/*ItemIndex*/, class UWidget*/*Entry*/);

protected:
	TWeakObjectPtr<class UPanelWidget> _panel;
	
	bool _bUsePanelSlot = true;
	TArray<TWeakObjectPtr<class UWidget>> _slotList;


public:
	FOnDynamicPanelSlotRefresh OnRefreshEntry;

public:
	FGsStaticPanelSlotHelper() = default;
	virtual ~FGsStaticPanelSlotHelper();

public:
	virtual void Clear();

public:
	// NativeOnInitialized 타이밍에 호출
	// bInUsePanelSlot: True: 패널에 배치된 슬롯 그대로 사용, False: AddSlot을 통해 다른 슬롯 등록
	void Initialize(class UPanelWidget* InPanel, bool bInUsePanelSlot = true);

	// 전체 갱신
	void RefreshAll(const int InItemCount);

	// 하나만 갱신하고 싶을 경우
	void RefreshByIndex(const int InIndex);	

	// bInUsePanelSlot가 false이면 별도로 슬롯을 등록해준다.
	void AddSlot(UWidget* InSlot);
};
