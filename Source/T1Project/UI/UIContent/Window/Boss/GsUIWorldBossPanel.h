#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Message/GsMessageBoss.h"
#include "Message/GsMessageUI.h"
#include "UTIL/GsTimeUtil.h"
#include "GameObject/Boss/GsWorldBossHandler.h"
#include "GsUIWorldBossPanel.generated.h"


UCLASS()
class T1PROJECT_API UGsUIWorldBossPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _bossListView;
	
	//boss image
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _nickNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _nameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _bossImage;

	//detail panel
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _detailPanelWidgetSwitcher;		
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIWorldBossDetailInfoPanel* _worldBossDetailPanel;

	//world boss button 
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _timePanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _timeSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _remainTimeTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnWorldBossEnter;

private:	
	TArray<const struct FGsSchemaWorldBossData*> _bossDataArray;
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossDelegates;
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegates;

private:
	FGsDateTimer _enterTimer;
	FDateTime _bossRoomOpenTime;
	FDateTime _bossSpawnTime;
	int _leftRewardCount;

public:
	const struct FGsSchemaWorldBossData* _selectWorldBossData;

public:
	TArray<class UGsWorldBossInfoListViewEntryData*> _bossEntryDataArray;
	bool _isCanEnter = false;

private:
	FText _ui_text_nPercent;
	FText _ui_text_canTeleport;
	FText _ui_text_canUseFunction;
	FText _ui_text_noticAlreadyEnter;
	FText _ui_text_notice_canNotEnter_inGuildDungeon;
	FText _ui_text_notice_canNotEnter_invadeAttack;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	

private:	
	TArray<class UGsWorldBossInfoListViewEntryData*> CreateBossArray();
	void InvalidNickName();	
	void InvalidBossImage();
	void StartRemainTime();	
	void CheckShowTimerType();
	void InvalidRemainTime();	
	void InvalidDetailPanel();
	void Clear();

	//world boss 
private:
	UFUNCTION()
		void OnClickEnterWorldBoss();

private:	
	void OnOpenBossKillContribution(const struct IGsMessageParam* inParam);

public:
	void SelectWorldBoss(const struct FGsSchemaWorldBossData* inBossInfo, int32 inLeftRewardCount);

private:	
	void OnSelectBoss(const struct IGsMessageParam* inParam);
	bool IsNeedToInvalid(const struct FGsUIMsgParamSelectBoss* inParam);	

public:	
	void InvalidateBossInfo();
	void InvalidBossList();	
	void Hide();
};