#pragma once

#include "CoreMinimal.h"
#include "GsUIPopupMonsterBookExpItem.h"
#include "../../Common/Icon/GsUIIconItem.h"
#include "GsUIPopupMonsterBookDetail.generated.h"


class UGsUIVFX;
class UGsButton;
class UTextBlock;
class UCanvasPanel;
class UPanelWidget;
class UWidgetSwitcher;
class UGsUIProgressBar;
class UGsUITooltipDesc;
class UGsUIIconItemReward;
class UGSUIMonsterCollectionPortrait;

UCLASS()
class T1PROJECT_API UGsUIPopupMonsterBookDetail : public UGsUIPopupMonsterBookExpItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGSUIMonsterCollectionPortrait* _portrait;

	// 2021/12/06 PKT - 몬스터 랭크
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRank;

	// 2021/12/06 PKT - 몬스터 공격 타입(선공/비선공)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textOffensive;

	// 2021/12/06 PKT - 종족
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCreatureRaceType;

	// 2021/12/06 PKT - 약점 / 재질
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDefenseMaterial;

	// 2021/12/06 PKT - 속성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textElement;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSpawnMap;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnShotMonsterDetail;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _progressBarExp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherUseItemPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelCompleteDimmed;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIIconItemReward* _iconRewardItem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherUnderButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOK;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApply;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _currentLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _applyLevelInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _applyLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _rewardBookPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _effectAmountChanged;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipKnowledgeStatus;

private:
	const class FGsMonsterKnowledgeBook* _data = nullptr;

	MonsterKnowledgeExp _increaseExp = INVALID_MONSTER_KNOWLEDGE_EXP;
	double _deltaExp = 0;

public:
	DECLARE_DELEGATE_OneParam(FOnClickedCloseDelegate, uint32);
	FOnClickedCloseDelegate _OnClickedCloseDelegate;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnBtnClickedChangedAmount() override;
	virtual void InvalidateAmount(int32 InAmountUsed) override;	

protected:
	UFUNCTION()
	void OnClose();

	UFUNCTION()
	void OnApply();

	UFUNCTION()
	void OnGoMonsterDetail();

	UFUNCTION()
	void OnTooltip();

	void OnLongPressRewardIcon(UGsUIIconItem& InIcon);
public:
	virtual void Invalidate() override;

public:
	void SetData(const class FGsMonsterKnowledgeBook* InData);
	const class FGsMonsterKnowledgeBook* GetData() const;

protected:
	virtual void OnInputCancel() override;
};
