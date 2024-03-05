// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GsUIHUDSideTab.generated.h"

class UGsButton;
class UWidgetSwitcher;
class UCanvasPanel;
/**
 * HUD 우측 퀘스트/ 파티 토글 탭 기능 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDSideTab : public UGsUIHUD
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickHUDSideTab, int32 /*buttonIndex*/, bool /*bIsSelected*/);

public:
	enum class EGsHUDSideTabType
	{
		NONE = -1,
		QUEST = 0,
		SCAN = 1,

		MAX,
	};

	struct FGsHUDSideTab
	{
		EGsHUDSideTabType _tabType;
		UGsButton* _button;
		UWidgetSwitcher* _switcher;

		//int index
		FGsHUDSideTab(EGsHUDSideTabType InType, UGsButton* InButton, UWidgetSwitcher* InSwitcher)
			: _tabType(InType), _button(InButton), _switcher(InSwitcher)
		{
		}

		bool SetToggle();
		bool IsSelected() const;
		void SetIsSelected(bool bIsSelected);
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnQuest;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherQuest;

	// 스캔 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnScan;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherScan;
protected:
	TArray<FGsHUDSideTab> _tabList;
	EGsHUDSideTabType _selectedTab = EGsHUDSideTabType::NONE;

public:
	FOnClickHUDSideTab OnClickSideTab;

protected:
	virtual void NativeOnInitialized() override;
	virtual void InvalidateAllInternal() override;

	// HUD모드에 따라 탭 제어를 하기 위함
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

protected:
	UFUNCTION()
	void OnClickQuest();

	UFUNCTION()
	void OnClickScan();

public:
	void SetSelectedIndex(EGsHUDSideTabType InType, bool bIsToggle = false);
	void OnClickNextTab();
	void OnClickPrevTab();
};
