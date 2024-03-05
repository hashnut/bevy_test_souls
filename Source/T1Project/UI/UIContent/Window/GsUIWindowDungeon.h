// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDelegateInstance.h"
#include "Message/GsMessageInput.h"
#include "Dungeon/GsDungeonUICaptureData.h"
#include "Dungeon/GsDungeonCommonData.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowDungeon.generated.h"

class UGsButton;
class UScrollBox;
class UPanelWidget;
class UHorizontalBox;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;
class UGsUIPageDungeonGroupBase;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowDungeon : public UGsUIWindow
{
	GENERATED_BODY()
	
protected:
	// 빽키 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UGsButton* _btnClose;

	// 2023/2/22 PKT - Main Tab Group
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _panelTabGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBoxSideTab;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetSideTab;

	// 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelperSideTab;
	
	// 2022/05/30 PKT - 디테일 창 스위처
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherDungeonDetail;
	
	// 2022/06/02 PKT - 파티 던전 RedDot
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _redDotPartyDungeon;

	// 2023/2/22 PKT - Active Widget
	TWeakObjectPtr<class UGsUIPageDungeonGroupBase> _activeDetailWidget;

protected:
	// 토글 그룹 Tab
	FGsToggleGroup _toggleGroupTab;
	// 토글 그룹 Slot
	FGsToggleGroup _toggleGroupSideSubTab;
	// 2022/06/14 PKT - UI 마지막 상태 저장(되돌아 오기)
	FGsDungeonUICaptureData _captureData;
	// 특정 맵 바로 가기
	int _shortcutMapId = 0;

	// 2023/6/7 PKT - Close Delegate
	TPair<MessageContents, FDelegateHandle> _contentsDelegate;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual bool SyncContentState() override;

protected:
	// 2022/06/03 PKT - Main Tab RedDot - 파티 던전만 존재
	void CheckRedDot();

	void OnCloseWindow();

private:
	UFUNCTION()
	void OnRefreshSideTabEntry(int32 InIndex, UWidget* InEntry);
	
	// 2022/05/31 PKT - 탭 메뉴 변경 시
	UFUNCTION()
	void OnSelectedMainTab(int32 inIndex);
	
	UFUNCTION()
	void OnSlectedSideSubTab(int InIndex/*, bool isSelected*/);
	
	UFUNCTION()
	void OnClickClose();

	// 슬롯 클릭 시
	void OnSelectedSideTab(const class FGsDungeonGroupBase* InSelectedData);

public:
	void Invalidate(DungeonGroupId InGroupId);

	void GoSelectedDungeonGroup(int32 inMapId);

	void InvalidatePartyList(const TArray<FGsPartyDungeonPartyStatusList>* InPartyStatusList);

	void InvalidateBattleArena();
};
