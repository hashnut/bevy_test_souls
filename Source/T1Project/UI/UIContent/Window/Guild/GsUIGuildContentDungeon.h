// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "Misc/Timespan.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "Message/GsMessageContents.h"
#include "Reward/GsRewardHelper.h"
#include "GsUIGuildContentDungeon.generated.h"

/**
 * 길드 던전 페이지
 */

UCLASS()
class T1PROJECT_API UGsUIGuildContentDungeon : public UGsUIGuildContentBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _guildDungeonNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _guildDungeonLevelTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _currentParticipantsUserCountTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _remainTimeTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _contributionRankTextBlock;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _currentParticipantsUserListView;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
		TSubclassOf<class UUserWidget> _currentParticipantsUserWidgetClass;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelEmptyUserList;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _enterDungeonButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _enteriesCountTextBlock;

	//기여도 순위
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _contributionListView;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
		TSubclassOf<class UUserWidget> _contributionWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsGuildDungeonContributionListViewEntry* _playerContribution;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _panelEmptyContributoinList;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _bossImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UProgressBar* _bossHpProgressBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _bossHpTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _bossHpWidget;

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _privateRewardPanel;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _privateRewardListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _privateRewardGuildButton;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _guildAcquireRewardPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _guildAcquireRewardListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _guildAcquireGuildButton;

private:	
	int32 _dungeonLevel = 1;
	class FGsGameStateGuild* _gameStateGuild;
	const struct FGsSchemaNpcData* _npcData;
	const struct FGsSchemaGuildDungeon* _guildDungeonData;
	class FGsGuildDungeonHandler* _guildDungeonHandler;
	TArray<TPair<MessageContentGuild, FDelegateHandle>> _guildDungeonMsgHandleArray;	
	FString _bossHpPercentText;
	bool _isClear;

	const float TIME_UPDATE_SEC = 0.5f;
	float _checkRemainSec = 0.f;
	time_t _dungeonResetTime = 0;
	


private:
	int32 _currentUserCount;
	int32 _maxEntriesCount;
	int32 _currentEntriesCount;
	int32 _contributionUserCount;
	int32 _maxBossHp;
	int32 _currentBossHp;	
	int32 _privateGuildCoin;	
	int32 _acquireRewardCoin;

private:
	FText _dungeonStageText;
	FText _dungeonCurrentPersonsText;
	FText _dungeonUserLevelText;
	FText _dungeonResetTicketTimeTextFormat;
	FText _dungeonEnterButtonText;
	FText _dungeonNRankText;
	FText _dungeonNoticEnterText;
	FText _dungeonRewardJoinListText;
	FText _dungeonRewardPersonListText;
	FText _dungeonRewardAcquireListText;
	FText _abnoramlTickerText;
	FText _dungoenCanNotJoinAllClearText;
	
	FText _minutFormat;
	FText _hourFormat;
	FText _dayFormat;

	FText _contributionRankText;

private:
	int32 _guildAcquireMoney;

private:
	TArray<const struct FGsSchemaRewardItemBagData*> _privateRewardItemDataList;	
	TArray<const struct FGsSchemaRewardItemBagData*> _rewardAcquireItemDataList;
	FGsRewardUnboxingData _privateRewardUnboxingData;
	FGsRewardUnboxingData _guildAcquireRewardUnboxingData;

public:
	virtual FString GetTabName() const override { return TEXT("Dungeon"); }

public:
	virtual void BeginDestroy() override;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& inMyGeometry, float inDeltaTime);
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	virtual void SendRequestPacket() override;
	virtual void Show(bool bIsMyGuild, bool inIsStackRecoverFlag = false) override;
	virtual void Hide() override;
	virtual void InvalidateAllInternal() override;
	void SetGuildDungeonHandler();
	void SetGuildDungeonData();
	void InvalidDungoenName();
	void InvalidParticipantUser();
	void InvalidRemainTime(FTimespan& inTimeSpan);

	void InvalidateRemainTime();

	void InvalidEntriesCount();
	void InvalidBossHp();
	void InvalidContribution();

private:
	void InitializeWidget();
	void InvalidBossImage();
	void InvalidReward();
	void InvalidPrivateReward();	
	void InvalidGuildAcquireReward();
	bool GetReward(const TArray<FGsSchemaRewardDataRow>& inRewardArray
	, TArray<const struct FGsSchemaRewardItemBagData*>& outRewardItemDataArray
	, FGsRewardUnboxingData& outRewardData);
	void InvalidRewardInternal(class UListView* inList, FGsRewardUnboxingData& inReward, TArray<const struct FGsSchemaRewardItemBagData*>& inBagData);				
	void OnInvalidGuildDungeon(const struct IGsMessageParam* inParam);

public:
	UFUNCTION()
		void OnClickEnterGuildDungeon();

private:
	// 아이콘 생성	
	void OnLongPressIcon(class UGsUIIconItem& InIcon);
	UFUNCTION()
		void OnClickPrivateReward();
	UFUNCTION()
		void OnClickGuildAcquireReward();	

	
};