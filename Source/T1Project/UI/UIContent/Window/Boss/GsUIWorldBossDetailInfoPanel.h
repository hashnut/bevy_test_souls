#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "GsUIWorldBossDetailInfoPanel.generated.h"


UCLASS()
class T1PROJECT_API UGsUIWorldBossDetailInfoPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	const int32 MAX_BOSS_TIME_NUM = 2;
	FTextKey UI_TITLE_MAILREWARD = TEXT("UI_Title_MailReward");
	FTextKey UI_TITLE_DROPREWARD = TEXT("Title_DropReward");
	const FTextKey UI_TEXT_NPERCENTINBRACKETS = TEXT("UI_Text_NPercentInBrackets");
	const FTextKey UI_TEXT_MONTH_DAY_HHMM = TEXT("UI_Text_MonthDayHHMM");
	const FTextKey UI_TEXT_NO_RESULT = TEXT("UI_Text_NoResult");

public:			
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _raceTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _materialTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _traitTextBlock;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _leftRewardCountTextBlock;

	//tooltip
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _toolTipButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUITooltipDesc* _toolTipPanel;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _spawnTimeListView;

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _rewardCompletePanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _participantRewardListView;

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _rankRewardListView;

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _lastHitRewardListView;

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _spawnIntervalTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _lastKillTimeTextBlock;	

private:
	TWeakObjectPtr<class UGsUIWindowBossInfo> _parent;
	EDayOfWeek _weekIndex;
	const struct FGsSchemaWorldBossData* _worldBossData;	
	const struct FGsSchemaNpcData* _bossNpcData;
	FText _ui_text_nPercentInBrackets;
	FText _ui_text_monthDayHHMM;
	FText _ui_text_noResult;	
	FText _ui_text_leftRewardCount;
	FText _ui_text_toolTipText;
	time_t _lastDeadTime = 0;		
	int _leftRewardCount = 0;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void SetWorldBossInfo(const struct FGsSchemaWorldBossData* inWorldBossData, int32 inLeftRewardCount);
	UFUNCTION()
		void OnChangeWeek(int32 inWeek);
	UFUNCTION()
		void OnClickToolTip();

private:
	void InvalidWorldBossDetailInfo();
	void InvalidSpawn(const struct FGsSchemaWorldBossData* inBossData);
	void InvalidParticipantReward(const struct FGsSchemaWorldBossData* inBossData);
	void InvalidRankReward(const struct FGsSchemaWorldBossData* inBossData);
	void InvalidLastHitReward(const struct FGsSchemaWorldBossData* inBossData);
	void InvalidRewardData(const TArray<FGsSchemaRewardDataRow>& inRewardList, class UListView* inRewardListView);
	void InvalidNpcInfo();	
	void InvalidLeftRewardCount();
	void Clear(EGsBossTab inTab);

public:
	void SetWeek(int32 inWeek);
	const struct FGsSchemaWorldBossData* GetWorldBossData();
	const struct FGsSchemaNpcData* GetBossNpcData();
	void SetParents(TWeakObjectPtr<class UGsUIWindowBossInfo> inParents);	

};

