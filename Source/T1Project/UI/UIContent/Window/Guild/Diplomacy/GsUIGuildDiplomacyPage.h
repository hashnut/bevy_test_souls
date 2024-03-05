// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIGuildDiplomacyPage.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UGsUIRedDotBase;
class UGsSwitcherButton;

class UGsUIGuildDiplomacyEnemy;
class UGsUIGuildDiplomacyAlly;
class UGsUIGuildDiplomacyState;
class UGsUIGuildDiplomacyBase;

struct FGsGuildMsgParamDiplomacyNotify;

/**
 * 기사단 외교 UI 
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDiplomacyPage : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnChangeDiplomacyTab);

public:
	enum EGsDiplomacyTabType
	{
		Enemy = 0,
		Ally = 1,
		AllyState = 2,
		OtherGuild = 3, // 내 길드 아닐 때 표시
		ALL = OtherGuild
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnEnemy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnAlly;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildDiplomacyEnemy* _uiPageEnemy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildDiplomacyAlly* _uiPageAlly;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildDiplomacyState* _uiPageState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotEnemy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotAlly;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotState;

protected:
	FGsToggleGroup _toggleGroupTab;
	TArray<class UGsUIGuildDiplomacyBase*> _pageList;
	TArray<UGsUIRedDotBase*> _redDotList;
	int32 _prevTabIndex = -1;
	int32 _currTabIndex = 0;
	bool _bIsMyGuild = false;

public:
	FOnChangeDiplomacyTab OnChangeDiplomacyTab;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:	
	void Show(bool bIsMyGuild);
	void Hide();

protected:
	void OnSelectTab(int32 InIndex);

	void SetUIPage(int32 InIndex);
	UGsUIGuildDiplomacyBase* GetCurrentPage() const;
	void UpdateRedDot(int32 InTabIndex);

public:
	// 권한 변동이 일어날 경우
	void OnChangeMyAuthority();
	void OnDiplomacyInvalidateList();
	void OnDiplomacyEnemyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam);
	void OnDiplomacyAllyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam);

	void OnDiplomacyResendList();
};
