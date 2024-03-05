// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/MessageParam/GsItemMessageParam.h"

#include "GsUIPopupCodexGemUse.generated.h"

class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UGsButton;
class UListView;
class UHorizontalBox;
class UTextBlock;
class UImage;
class UGsHorizontalBoxIconSelector;
class UGsWrapBoxInventoryIconSelector;
class UGsUITooltipDesc;
class UPaperSprite;
class UTexture2D;
class UGsDynamicIconSlotHelper;
class UGsUIIconItem;
class UGsUIProgressBar;
class FGsCostPackage;
class UGsUIVFX;
class UPanelWidget;
class UWidgetSwitcher;

/**
 * 사운드 연출 리소스 이름 (DTGsSoundResData 로부터 조회)
 */
const FString GEM_SOUND_USE_LEVEL_UP = TEXT("UI_MB_GemUse_LevelUp"); // (지식의 결정 사용으로 레벨업 시 파티클 사운드)
const FString GEM_SOUND_CLICK = TEXT("Snd_UI_Click"); // (버튼 클릭)
const FString GEM_SOUND_USE_GEM = TEXT("UI_MB_SlateComplete"); // (지식의 결정 사용)

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCodexGemUse : public UGsUIPopup
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnClickedCancelDelegate);
	FOnClickedCancelDelegate OnClickedCancelDelegate;

public:
	struct Parameters
	{
		CodexCategoryId _currentCategoryId;
		CodexMapGroupId _currentMapGroupId;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollCategoryView;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCategoryView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCategoryView;

	/** 카테고리 토글 그룹 */
	FGsToggleGroup _toggleGroupCategoryView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMapGroupName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMapGroupLevel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelCanLevelUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLeftMapGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageMapGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRightMapGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textExpComplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _progressBarExp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxInventoryIconSelector* _iconSelector;

	/** 지식의 결정 아이콘 슬롯 헬퍼 */
	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelperIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcReset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDecreaseTen;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDecrease;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCurrentAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIncrease;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIncreaseTen;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcMax;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewCodexGemRequirement;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApply;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipCodexGemUse;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _expCompletedHideUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _effectAmountChanged;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherBottomArea;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxGemUseLevelSame;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxGemUseLevelUp;

private:
	
	/** 대륙 카테고리 인덱스 */
	int32 _currentTabIndex;

	/** 대륙 카테고리 */
	int32 _currentCategoryId;

	/** 맵 그룹 인덱스 */
	int32 _currentMapGroupIndex;
	
	/** 맵 그룹 인덱스 */
	int32 _currentMapGroupId;

	/** Lv.{0} 캐싱 */
	FText _levelText;

	/** 지식 초과 메시지 캐싱 */
	FText _knowledgeGemBurnText;

	/** 사용하고자 하는 지식의 결정 아이템 Id */
	ItemId _knowledgeGemId;

	/** 사용하고자 하는 지식의 결정 갯수 */
	int32 _knowledgeItemCount;

	/** 사용하고자 하는 지식의 결정 갯수의 보유량 */
	int32 _knowledgeGemOwnAmount;

	/** 사용하고자 하는 지식의 결정 증가량 */
	int32 _knowledgeGainAmount;

	/** 사용 가능한 지식의 결정 최대 갯수 (현재 MapGroup 의 Exp 기준으로 최대치 계산) */
	int32 _maxUsableKnowledgeGem;

	/** 선택한 지식의 결정 (갯수가 있을 때만 선택 가능) */
	int32 _selectedKnowledgeGemIndex;

	/** 지식의 결정 GainAmount 가 잔여 지식량보다 클때, 소각되는 양 */
	int32 _knowledgeGainAmountBurn;

	/** 맵 그룹의 Exp 가 변하는 속력(초속이며, 절댓값) = |(목표 Exp - 현재 Exp)| / (소요 시간) */
	float _rateExpChangeForSecond;
	
	/** 맵 그룹의 Exp 가 변하는 속력 최소 수치 */
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float BASE_RATE_EXP_CHANGE_FOR_SECOND = 100.f;

	/** 맵 그룹의 Exp 가 변하는데 걸리는 시간(1개 짜리. 길게 변경할 때는 LONG 버전 사용) */
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float TIME_ELAPSED_PROGRESS_BAR_SHORT = 0.11f;

	/** 맵 그룹의 Exp 가 변하는데 걸리는 시간. 얼마를 변경하든, 0.66 초가 걸린다. 왜 0.66 초인가? 그 이유는 */
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float TIME_ELAPSED_PROGRESS_BAR_LONG = 0.66f;

	/** 맵 그룹의 현재 누적 수치*/
	int32 _currentMapGroupExp;

	/** 맵 그룹의 목표 누적 수치 */
	int32 _targetMapGroupExp;

	/** 맵 그룹의 목표 누적 수치를 위해 변동하고 있는 수치 */
	float _ongoingMapGroupExp;

	/** 현재 선택된 맵 그룹의 최대 누적 경험치 */
	int32 _maxAccumulateExpForCurrentMapGroup = 0;

private:

	/** 지식 획득에 대응 */
	TArray<TPair<MessageContentMonsterKnowledge, FDelegateHandle>> _monsterKnowledeDelegates;

	/** 아이템 갯수 변화에 대응 */
	TArray<TPair<MessageItem, FDelegateHandle>> _itemActionDelegates;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/** 오픈 시, 카테고리 업데이트 */
	void InvalidateCategoryTab();

	/** 맵 그룹 변경 시 업데이트 */
	void InvalidateMapGroupArea();

	/** 맵 그룹 레벨 업데이트 (레벨 업 가능 여부까지) */
	void InvalidateMapGroupLevelText();

	/** 맵 그룹 이미지 업데이트 */
	void InvalidateMapGroupImage();

	/** 맵 그룹에 사용 가능한 아이템 목록 업데이트 (오직 맵 그룹이 바뀔때만 불린다) */
	void InvalidateUsableItemList(int32 InSelectedIndex = -1);

	/** 맵 그룹에 사용 가능한 아이템 갯수 및 영역 업데이트 */
	void InvalidateGemCount();

	/** 맵 그룹 프로그레스바 업데이트 */
	void InvalidateProgressBar(float InDeltaTime = 0.f);

	/** 맵 그룹 지식 단계 업데이트 */
	void InvalidateKnowledgeLevelList();

	/** 지식의 결정 사용이 가능한지 여부를 체크 */
	void InvalidateKnowledgeGemApplyState();

	/** 지식의 결정 아이템 사용 영역 가리는 여부 체크하여 업데이트 */
	void InvalidateBottomArea();

	/** 최대 가용 지식의 결정 갯수 및 소각 예상량 계산 */
	void CalculateMaxUsableGemCountAndBurn();

	/** 목표 경험치 세팅 (진행 경험치는 현재 경험치로 즉시 세팅된다) */
	void CalculateExp(bool InIsForce = true, float InTimeElapsed = 0.11f);

	/** 지식의 결정 사용 시, 업데이트할 내용을 담은 함수 */
	void OnGainKnowledge(const IGsMessageParam* InParam = nullptr);

	/** 지식의 결정 사용 시, 업데이트할 내용을 담은 함수 */
	void OnGainKnowledgeGem(const IGsMessageParam* InParam = nullptr);

	/** 지식의 결정 사용 시, 맵 그룹 이미지 영역에 보여줄 연출 (레벨 업하는 경우와 그렇지 않은 경우를 구분) */
	void PlayGemUseEffect();

private:
	/** 지식의 결정 비용 만들기 */
	TSharedRef<FGsCostPackage> MakeCostPackage();

	/** 아이템이 업데이트 되었을 때, 해당 아이템 중 지식의 결정이 포함되었는지 여부 체크 */
	bool IsItemGem(TArray<ItemDBId> InItemDBIdList);
	bool IsItemGem(ItemDBId InItemDBId);

protected:
	void SetImageSprite(UPaperSprite* InSprite);
	void SetImageTexture(UTexture2D* InTexture);

public:
	void SetParameters(Parameters InParameters);


protected:
	UFUNCTION()
	void OnRefreshEntryCategoryView(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnSelectChangedCategoryView(int32 InIndex);

	UFUNCTION()
	void OnRefreshGemIconItem(int32 InIndex, UGsUIIconBase* InIcon);
	
	UFUNCTION()
	void OnClickedLeftMapGroup();

	UFUNCTION()
	void OnClickedRightMapGroup();

	UFUNCTION()
	void OnClickedCalcReset();

	UFUNCTION()
	void OnClickedDecreaseTen();

	UFUNCTION()
	void OnClickedDecrease();

	UFUNCTION()
	void OnClickedIncrease();

	UFUNCTION()
	void OnClickedIncreaseTen();

	UFUNCTION()
	void OnClickedCalcMax();

	UFUNCTION()
	void OnClickedApply();

	UFUNCTION()
	void OnClickedCancel();

	void OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon);
	
	void OnClickedItemIcon(UGsUIIconItem& InIcon);

	/** 키보드 입력으로 닫는 기능 구현을 위함 */
	virtual void OnInputCancel() final;

	UFUNCTION()
	void OnPressedBtnTooltip();

	/** 아이템 */
	void OnAddItemList(FGsItemMessageParamBase& InParam);

	void OnUpdateItem(FGsItemMessageParamBase& InParam);

	void OnRemoveItem(FGsItemMessageParamBase& InParam);
};
