#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GSUIPopupBattleArenaSeasonRankBoard.generated.h"

UCLASS()
class  T1PROJECT_API UGSUIPopupBattleArenaSeasonRankBoard : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 2024/1/23 PKT - Main Tab Button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _btnSeasonRank;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _btnSeasonReward;

	// 2024/1/23 PKT - Main Board( 랭크 / 보상 )
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherMainBoard;

	// 2024/1/30 PKT - Season 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textSeasonName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textSeasonEndDate;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _ritchTextSeasonRewardNotice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _ritchTextRicPlayCount;

	/**
	 * Tier Rank
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBoxTierCategory;
	// 2024/1/24 PKT - Tier Cateogry
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _tierCategoryRoot;

	// 2024/1/24 PKT - Tier Rank List View Switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherTierRankList;

	// 2024/1/24 PKT - Tier Rank List View
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _listViewTierRankItem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textEmptyReason;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _widgetMySeasonRank;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIBattleArenaSeasonRankItemEntry* _widgetMyRankData;


	/*
	* Tier / Rank Reward
	*/
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _btnTeirReward;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _btnRankReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherRewardList;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBoxTierRewardListRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBoxRankRewardListRoot;
	

	// 2024/1/23 PKT - 확인 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSeasonDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnConfirm;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _tierCategoryEntryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _tierCategoryPanelSlotHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _tierRewardEntryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _tierRewardPanelSlotHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _rankRewardEntryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _rankRewardPanelSlotHelper;
	
protected:
	// 2024/1/25 PKT - Rank Data Pool
	UPROPERTY()
	class UGsPoolUObject* _poolRankItemData;

	// 2024/1/23 PKT - Main Tab Group
	FGsToggleGroup _toggleMainTabGroup;
	// 2024/1/23 PKT - Tier Category Group
	FGsToggleGroup _toggleTierCategoryGroup;
	// 2024/1/23 PKT - Reward Tab Group
	FGsToggleGroup _toggleRewardTabGroup;

protected:
	// 2024/1/23 PKT - 시즌 정보 타입(개인/기사단)
	RankSearchType _rankSearchType = RankSearchType::MAX;

	MsgContentBattleArenaRankArray _battleArenaRankDelegates;
	
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void BindMessage();
	void UnBindMessage();

	void OnDelegateInvalidateLocalData(const struct IGsMessageParam* InParam);
	void OnDelegateResetRankData(const struct IGsMessageParam* InParam);
	void OnDelegateCreateRankList(const struct IGsMessageParam* InParam);
	void OnDelegateInvalidateRankData(const struct IGsMessageParam* InParam);

	
	void BuildMainRankView(bool IsMyRankFocus = false);
	void BuildMainRewardView();
	void BuildTierRankList(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId);	
	void BuildMySeasonTierData();

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelectedMainTab(int32 index);
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelectedTierCategory(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelectedRewardTab(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedConfirm();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshTierCategory(int32 InIndex, UWidget* InEntry);
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshTierReward(int32 InIndex, UWidget* InEntry);
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshRankReward(int32 InIndex, UWidget* InEntry);

protected:
	virtual void OnInputCancel() override;

public:
	void SetData(RankSearchType InRankSearchType);
};
