#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Message/GsMessageBoss.h"
#include "Message/GsMessageUI.h"
#include "GsUIFieldBossPanel.generated.h"

UCLASS()
class T1PROJECT_API UGsUIFieldBossPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _bossListView;

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _contributionButton;	

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
		class UGsUIFieldBossDetailInfoPanel* _fieldBossDetailPanel;

public:
	//mutant
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _mutantPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsSwitcherButton* _mutantSwitcherButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _mutantPercentTextBlock;

private:	
	TArray<const struct FGsSchemaFieldBossData*> _bossDataArray;	
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossDelegates;
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegates;

public:
	const struct FGsSchemaFieldBossData* _selectFieldBossData;
	const struct FGsSchemaFieldBossData* _selectFieldBossMutantData;
	bool _isShowFieldBossMutant = false;	
	time_t _fieldBossLastDeadTime = 0;

public:
	TArray<class UGsFieldBossInfoListViewEntryData*> _bossEntryDataArray;

private:
	FText _ui_text_nPercent;
	FText _ui_text_canTeleport;
	FText _ui_text_canUseFunction;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	

private:	
	bool TryCreateBossArray(OUT TArray<class UGsFieldBossInfoListViewEntryData*>& outBossArray);
	void InvalidNickName();
	void InvalidMutantInfo();	
	void InvalidBossImage();	
	void InvalidDetailPanel();
	void Clear();

private:
	void OnOpenBossKillContribution(const struct IGsMessageParam* inParam);
	UFUNCTION()
		void OnReqBossKillConstribution();	
	UFUNCTION()
		void OnSwitchMutant();

public:
	void SelectBoss(const struct FGsSchemaFieldBossData* inBossInfo, const FGsSchemaFieldBossData* inMutant, bool inIsShowMutant = false);

private:	
	void OnSelectBoss(const struct IGsMessageParam* inParam);
	bool IsNeedToInvalid(const struct FGsUIMsgParamSelectBoss* inParam);			
	void OnChangeFieldbossBookmark(const struct IGsMessageParam* inParam);

public:
	void InvalidateBossInfo();
	void InvalidBossList();	

public:		
	const struct FGsSchemaFieldBossData* GetSelectBossData();
};