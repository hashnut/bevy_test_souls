// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GsUIWindowInvade.generated.h"

class UGsButton;
class UTextBlock;
class UScrollBox;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;

struct FGsInvadeMap;

UCLASS()
class T1PROJECT_API UGsUIWindowInvade : public UGsUIWindow
{
	GENERATED_BODY()

	enum class ContetnsViewType
	{
		InvadeDesc,
		InvadeTitle,
	};

protected:
	// 2021/11/23 PKT - 뒤로 가기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	// 2021/11/23 PKT - 닫기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnInvadeDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnInvadeTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherContentsView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxBattleTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBattleTitleName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCurrencyItemCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAttackCurrencyItemCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _texDefenceCurrencyItemCount;
		
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperBattleTitle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfBattleTitleEntry;

	FGsToggleGroup _toggleGroupMainTabs;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _playTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _invadeMapInfoSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryInvadeMapWidget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxInvadeMap;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _invadeMapPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _activeMapPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyMapPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _invadeMapListHelper;

	TArray<TWeakPtr<FGsInvadeMap>> _activePortalInMaps;
	bool _reqPortalInfo{ false };

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool SyncContentState() override;

protected:
	void OnSelectedBtnMainCategory(int32 InIndex);

	UFUNCTION()
	void OnRefreshEntryBattleTitle(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntryInvasionMap(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnCreateEntryInvasionMap(UWidget* InEntry);

	void OpenInvasionWorldMap(MapId mapId);

protected:
	void InvalidateView();
	void InvalidateDesc();
	void InvalidateTitle();

public:
	void InvalidateBattleBadge();
	void InvasionPortalSpawnInfo();
};
