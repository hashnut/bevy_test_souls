// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"

class UWidget;

/**
 * FGsToggleGroup
 * : 동일 Panel 밑에있는 토글버튼들의 그룹 관리. 항상 한 개만 선택(예: 탭 버튼).
 * - Panel 밑의 자식들은 IGsToggleGroupEntry 상속 항목 이어야 한다.
 */
class T1PROJECT_API FGsToggleGroup final
{
public:

	//이전 토글 그룹 나갈때 이벤트
	DECLARE_DELEGATE_OneParam(FOnToggleGroupPreLeave, int32 /*InIndex*/);

	// 선택이 바뀌었을 때
	DECLARE_DELEGATE_OneParam(FOnToggleGroupSelectChanged, int32 /*InIndex*/);
	
	// 클릭이 된 슬롯의 인덱스를 리턴
	DECLARE_DELEGATE_TwoParams(FOnToggleSlotClicked, int32 /*InIndex*/, bool /*bIsSelected*/);
	
	// 클릭이 된 슬롯이 잠겨 있음을 알림
	DECLARE_DELEGATE_OneParam(FOnToggleGroupChangeBlocked, int32 /*InIndex*/);

	// 블록해야하는지 체크 하는 함수 
	DECLARE_DELEGATE_RetVal_OneParam(bool, FOnToggleBlockCheck, int32 /*InIndex*/);

	//같은 것을 선택했을 때 이벤트
	DECLARE_DELEGATE(FOnToggleGroupNotChange);

protected:
	int32 _selectedIndex = 0;
	TArray<TWeakObjectPtr<UWidget>> _toggleList; // IGsToggleGroupEntry 상속 항목
	TSet<int32> _blockedIndexSet;

public:
	FOnToggleGroupPreLeave OnLeavePreSelected;
	FOnToggleBlockCheck OnCheckBlock;
	FOnToggleGroupSelectChanged OnSelectChanged;
	FOnToggleSlotClicked OnSlotClicked;
	FOnToggleGroupChangeBlocked OnChangeBlocked;
	FOnToggleGroupNotChange OnNotChangeEvent;

public:
	FGsToggleGroup() = default;
	~FGsToggleGroup();

public:
	void Clear();

public:
	// 패널의 자식들을 슬롯으로 
	void AddToggleWidgetByParentPanel(class UPanelWidget* InPanel, bool bIsDynamic = true);
	IGsToggleGroupEntry* RecursiveFindToggleWidget(UWidget* InWidget);
	void AddToggleWidget(IGsToggleGroupEntry* InWidget);
	IGsToggleGroupEntry* GetEntry(int32 InIndex);

	void SetSelectedIndex(int32 InIndex, bool bInForced = false);
	int32 GetSelectedIndex() const { return _selectedIndex; }
	void SetSelectedIndexNoEvent(int32 InIndex);

protected:
	void OnToggleWidgetStateChanged(int32 InIndex, bool bIsSelected);

public:
	// 항상 한 개의 슬롯이 선택되게 하는 로직
	void ProcessSelectOnlyOne(int32 InIndex);

public:
	void AddBlockedIndex(int32 InIndex);
	void DeleteBlockedIndex(int32 InIndex);
	bool IsBlockedIndex(int32 InIndex);

public:
	void SetVisibilityEntry(int32 InIndex, ESlateVisibility InVisibility);
};
