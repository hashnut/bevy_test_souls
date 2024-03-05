#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "GsUIFieldBossDetailInfoPanel.generated.h"

class UImage;
class UTextBlock;
class UGsButton;
class UListView;
struct FGsSchemaFieldBossData;
struct FGsSchemaNpcData;
class UGsUIWindowBossInfo;

UCLASS()
class T1PROJECT_API UGsUIFieldBossDetailInfoPanel : public UUserWidget
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
		UImage* _bossIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _bossNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _areaNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _raceTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _materialTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _traitTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _channelTextBlock;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _worldMapButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _mailRewardButton;	

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _spawnTimeListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _mailRewardListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _dropRewardListView;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UVerticalBox* _mailRewardPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UVerticalBox* _dropRewardPanel;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _spawnTimeWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _spawnIntervalTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _lastKillTimeTextBlock;
	// 등장 시간/ 등장 간격
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSpawnTimeType;

private:
	TWeakObjectPtr<UGsUIWindowBossInfo> _parent;
	EDayOfWeek _weekIndex;
	const FGsSchemaFieldBossData* _bossData;
	const FGsSchemaFieldBossData* _bossMutant;
	const FGsSchemaNpcData* _bossNpcData;
	FText _ui_text_nPercentInBrackets;
	FText _ui_text_monthDayHHMM;
	FText _ui_text_noResult;
	bool _isShowMutant = false;
	time_t _lastDeadTime = 0;	

private:
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegates;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void SetFieldBossInfo(const FGsSchemaFieldBossData* inBossInfo, const FGsSchemaFieldBossData* inMutant, bool inShowMutant = false, time_t inTime = 0);
	UFUNCTION()
		void OnChangeWeek(int32 inWeek);
	UFUNCTION()
		void OnClickWorldMap();
	UFUNCTION()
		void OnClickMailReward();

private:
	void InvalidBossDetailInfo();
	void InvalidSpawn();
	void InvalidMailReward(const FGsSchemaFieldBossData* inBossData);
	void InvalidDropReward(const FGsSchemaFieldBossData* inBossData);
	void InvalidNpcInfo();
	void InvalidChannel();	
	void Clear();

private:
	void OnInvalidLastDeathTime(const struct IGsMessageParam* inParam);

public:
	void SetWeek(int32 inWeek);
	const FGsSchemaFieldBossData* GetBossData();
	const FGsSchemaNpcData* GetBossNpcData();
	void SetShowMutant(bool inIsShowMutant);

};

