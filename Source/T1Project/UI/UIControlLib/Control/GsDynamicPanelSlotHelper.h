// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsDynamicPanelSlotHelper.generated.h"

/**
 * UGsDynamicPanelSlotHelper
 * : Entry슬롯의 동적관리를 도와주는 클래스.
 */
UCLASS()
class T1PROJECT_API UGsDynamicPanelSlotHelper : public UObject
{
	GENERATED_BODY()	

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDynamicPanelSlotCreate, class UWidget*, Entry);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDynamicPanelSlotRefresh, int32, ItemIndex, class UWidget*, Entry);

protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> _entryWidgetClass;

	TWeakObjectPtr<class UPanelWidget> _panel;
	TWeakObjectPtr<class UPanelWidget> _emptyPanel;	
	bool _bUseEmptyPanel = false;
	int32 _itemCount = 0;

public:
	FOnDynamicPanelSlotCreate OnCreateEntry;
	FOnDynamicPanelSlotRefresh OnRefreshEntry;

public:
	// NativeOnInitialized 타이밍에 호출
	void Initialize(TSubclassOf<UUserWidget> InEntryClass, class UPanelWidget* InPanel);

	// 전체 갱신
	void RefreshAll(const int InItemCount);
	// 2021/08/05 PKT - 범위 기반으로 갱신
	void RefreshRange(const int32 InBegin, const int32 InRange);
	// 하나만 갱신하고 싶을 경우
	void RefreshByIndex(const int InIndex);

	UWidget* GetEntry(const int InIndex);
	int32 GetItemCount() const { return _itemCount; }
	int32 GetChildrenCount() const;

	void SetEmptyListPanel(class UPanelWidget* emptyPanel, bool useEmptyPanel = true);

public:
	void AllClearChildren();
};
