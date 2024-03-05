// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsDynamicIconSlotHelper.generated.h"

class IGsIconSelectorInterface;
class UGsUIIconBase;

/**
 * UGsDynamicIconSlotHelper
 * : Entry슬롯의 동적관리를 도와주는 클래스. 
 * - IGsIconSelectorInterface 이용 아이콘들
 * - 동일 아이콘을 쓴다는 가정이 있어야 사용할 수 있다.
 */
UCLASS()
class T1PROJECT_API UGsDynamicIconSlotHelper : public UObject
{
	GENERATED_BODY()	

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDynamicIconSlotRefresh, int32, ItemIndex, UGsUIIconBase*, IconEntry);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDynamicIconSlotCreate, UGsUIIconBase*, IconEntry);

protected:
	TWeakObjectPtr<class UPanelWidget> _panel;

public:
	FOnDynamicIconSlotCreate OnCreateIcon;
	FOnDynamicIconSlotRefresh OnRefreshIcon;

public:
	virtual void BeginDestroy() override;
	

public:
	// NativeOnInitialized 타이밍에 생성 직후 호출.
	void Initialize(IGsIconSelectorInterface* InSelector);

	void Clear();

	// 전체 갱신
	void RefreshAll(const int InItemCount);

	// 하나만 갱신하고 싶을 경우
	void RefreshByIndex(const int InIndex);

	// 아이콘 생성만 함
	void CreateIcon(const int InItemCount);
	void RefreshAll();

	// 한번에 처리하지 않고 InCreateCount 만큼 Create 처리
	void CreateIconByTick(const int32 InCreateCount, const int32 InMaxCount);
	void RefreshIconByTick(const int32 InRefreshCount, const int32 InStartIndex);

	UGsUIIconBase* GetIcon(const int InIndex);
	IGsIconSelectorInterface* GetIconSelector() const;
};
