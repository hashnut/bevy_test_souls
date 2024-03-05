// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/AgitDungeon/GsUIAgitDungeonPageBase.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Classes/GsPoolUObject.h"
#include "Reward/GsRewardHelper.h"
#include "Reward/GsRewardIconDataWrapper.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "GsUIAgitDungeonPageRaid.generated.h"

class UImage;
class UScrollBox;
class UTextBlock;
class UPanelWidget;
class UListView;
class UWidgetSwitcher;
class UProgressBar;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsUICurrencyButton;
class UGsUITooltipDesc;
class UGsUIGuildDungeonRankingListEntry;
class FGsAgitDungeonRaidBossData;
class FGsAgitDungeonRaidData;
struct FGsSchemaRewardData;
struct FGsRewardUnboxingData;
struct FGsSchemaAgitDungeonRaid;
class UGsRewardIconCommonListItem;
class UGsGuildDungeonRankingListItem;
class UGsGuildDungeonPlayerListItem;
struct IGsMessageParam;

/**
 * 기사단 영지 던전 - 공략던전
 */
UCLASS()
class UGsUIAgitDungeonPageRaid : public UGsUIAgitDungeonPageBase, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUIAgitRaid")
	TSubclassOf<class UUserWidget> _raidCountEntryClass;

	// 보스메뉴
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootMenu;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherMenu;

	// 공략 횟수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRaidCountInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipRaidCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRaidCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollRaidCount;	

	// 보스정보 탭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBossName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBossSubName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgBoss;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootBossHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barBossHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBossState;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBossRemainTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelBossClear;
	
	// 보스 목록 탭
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUIAgitRaid")
	TSubclassOf<class UUserWidget> _bossEntryClass;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBossList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildAsset;

	// 하단 버튼
	// 0: 공략시작, 1: 공략 종료, 2: 공략 불가능
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDungeonBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnDungeonOpen;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDungeonClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDungeonOpenInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipDungeonOpen;

	// 기여도 순위 UI
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyRankngList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsGuildDungeonRankingListItem))
	UListView* _listViewRanking;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildDungeonRankingListEntry* _myRankingUI;

	// 입장 인원 UI
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPlayerCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyPlayerList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsGuildDungeonPlayerListItem))
	UListView* _listViewPlayer;
	 
	// 개인 획득보상
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInfoRewardPersonal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsRewardIconCommonListItem))
	UListView* _listViewRewardPersonal;
	 
	// 기사단 획득 보상
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInfoRewardGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsRewardIconCommonListItem))
	UListView* _listViewRewardGuild;

	// 던전 입장
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockResetTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEnterRemainCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEnter;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _helperRaidCount;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _helperBossList;

	UPROPERTY()
	UGsPoolUObject* _poolRankingListItem;
	UPROPERTY()
	UGsPoolUObject* _poolPlayerListItem;
	UPROPERTY()
	UGsPoolUObject* _poolRewardListItem;

protected:
	FGsToggleGroup _toggleGroupMenu;
	FGsToggleGroup _toggleGroupBoss;

protected:
	bool _bIgnoreChangeSelect = false;
	bool _bIsForcedRequestBossInfo = false;
	int32 _selectedRaidId = 0;
	int32 _remainRaidCount = 0;
	TArray<TWeakPtr<FGsAgitDungeonRaidBossData>> _bossList;

	float _checkBossCloseTime = 0.f;
	time_t _bossCloseTime = 0;
	float _checkResetTicketTime = 0.f;
	time_t _resetTicketTime = 0;	
	float _checkDelayedResetTime = 0.f;

	FGsRewardUnboxingData _unboxingPersonal;
	FGsRewardUnboxingData _unboxingGuild;

	// 부자연 스러운 티커 출력을 방지하기 위함
	AgitDungeonRaidState _lastRequestState = AgitDungeonRaidState::NONE;
	
	// 깜빡임 방지 위함
	FString _strBossImgPath;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override;

public:
	virtual void Show() override;

	virtual void OnResetTimeContents(ResetTimeTicket InType) override;
	virtual void OnChangeCurrency(CurrencyType InType) override;
	virtual void OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam) override;

	// 메시지 연결
public:	
	void OnRcvRaidInfo();
	void OnRcvRaidBossInfo(int32 InRaidId);
	void OnRcvRaidBookmarkList();
	void OnRcvRaidBookmarkChanged();
	void OnRcvRaidOpenChanged(const IGsMessageParam* InParam);

private:
	void OnSelectMenu(int32 InIndex);
	void OnSelectBoss(int32 InIndex);

	UFUNCTION()
	void OnRefreshRaidCount(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshBossList(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnPressRaidCountInfo();
	UFUNCTION()
	void OnPressDungeonOpenInfo();

	void OnClickOpenDungeon();
	UFUNCTION()
	void OnClickCloseDungeon();

	UFUNCTION()
	void OnClickRewardPersonal();
	UFUNCTION()
	void OnClickRewardGuild();
	UFUNCTION()
	void OnClickEnterDungeon();

private:
	void RefreshBossList(bool bSelectFirst);

	// 보스 선택과 상관없는 항목들
	void InvalidateGuildAsset();

	void InvalidateRaidCount();
	void InvalidateResetTime(bool bCheckZero);
	
	void InvalidateBossList();

	// 보스가 선택 된 뒤의 항목들
	void InvalidateBossInfo();
	void InvalidateBossRemainTime(bool bCheckZero);
	void InvalidateOpenButton();

	void InvalidateRanking();
	void InvalidatePlayers();
	void InvalidateReward();
	void InvalidateEnterButton();

private:
	void CollectReward(const FGsSchemaAgitDungeonRaid* InTable, bool bIsGuildReward, OUT FGsRewardUnboxingData& OutData);
	void SetRewardListView(FGsRewardUnboxingData& InData, OUT UListView* OutListView);
	void OpenRewardPopup(bool bIsGuildReward);

	// 전체 갱신하는 상황
	void ForcedRefreshAll(bool bShowTicker, bool bClosePopup);

private:
	// 0 이하 입력시 비움
	void SetBossHp(float InPercent);
	void SetBossImage(class UTexture2D* InTexture);
	bool CheckOpenMemberGrade(bool bShowTicker) const;

public:
	void SetLastRequestState(AgitDungeonRaidState InState) { _lastRequestState = InState; }

private:
	FGsAgitDungeonRaidData* GetRaidData() const;
	FGsAgitDungeonRaidBossData* GetSelectedBossData() const;
	FGsAgitDungeonRaidBossData* GetBossData(int32 InBossId) const;
	int32 GetSelectedBossIndex() const;	
};
