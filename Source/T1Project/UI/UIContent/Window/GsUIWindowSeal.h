#pragma once

#include "Runtime/CoreUObject/Public/UObject/Interface.h"

#include "Management/ScopeGame/GsSealManager.h"
#include "Seal/GsSealSlot.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UILib/Base/GsUIWindow.h"

#include "GsUIWindowSeal.generated.h"

class FGsContentsGameBase;
class FGsSealNode;
class FGsSealPack;
class FGsSealSlot;
class UGsButton;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsSwitcherButton;
class UGsToggleButton;
class UGsUICurrencyButton;
class UGsUIIconBase;
class UGsUIIconItem;
class UGsUIProgressBar;
class UGsUISealLevelStatListItem;
class UGsUISealNode;
class UGsUISealNodeLine;
class UGsUISealPack;
class UGsUISealPackGainListItem;
class UGsUISealSlot;
class UGsUISealSlotListEntry;
class UGsUISealSlotListItem;
class UGsUISequencePlayer;
class UGsUITooltipDesc;
class UGsUIVFX;
class UGsUIWindowSeal;
class UGsWidgetAnimationPlayCallbackProxy;
class UListView;
class URichTextBlock;
class UTextBlock;
class UUMGSequencePlayer;
class UUserWidget;
class UWidget;
class UWidgetAnimation;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
class UScrollBox;
struct FGeometry;

/** 봉인 연출 이벤트 목록 */
UENUM()
enum class EGsUISealWindowEvent : uint8
{
	Reset,
	SealNode,
	SealNodeValue,
	SealSlot0,
	SealSlot1,
	SealSlot2,
	SealSlotValue0,
	SealSlotValue1,
	SealSlotValue2,
	SealLines,
	SealLinesValue,
};

/** 봉인 시퀀스연출이벤트 발생 시, 각 객체로 전달할 파라미터 */
USTRUCT()
struct FGsUIWindowSealEventParameters
{
	GENERATED_BODY()
public:
	EGsUISealWindowEvent _event;
	SealPackId _packId = INVALID_SEAL_PACK_ID;
	SealId _nodeId = INVALID_SEAL_ID;
	// int32 _nodeIndex = -1;
	int32 _slotIndex = -1;
	bool _isSkip = false;
};

/** 봉인 시퀀스연출이벤트 처리 U 인터페이스 */
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UGsUIWindowSealEvent : public UInterface
{
	GENERATED_BODY()
};

/** 봉인 시퀀스연출이벤트 처리 인터페이스 */
class IGsUIWindowSealEvent
{
	GENERATED_BODY()

public:
	virtual void OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters) = 0;
};

/**
 * 봉인 윈도우
 */
UCLASS()
class UGsUIWindowSeal final : public UGsUIWindow
{
public:
	DECLARE_EVENT_OneParam(UGsUIWindowSeal, FGsOnUIEvent, const FGsUIWindowSealEventParameters&)

private:
	using MatchCount = FGsSealManager::MatchCount;

	GENERATED_BODY()

public:
	/** 초기화 파라미터 */
	struct Parameters final
	{
		SealPackId _selectedPackId;

		Parameters()
			: _selectedPackId(INVALID_SEAL_PACK_ID)
		{
		}

		explicit Parameters(const SealPackId InSealPackId)
			: _selectedPackId(InSealPackId)
		{
		}

		bool operator==(const UGsUIWindowSeal::Parameters& InRhs) const
		{
			return (InRhs._selectedPackId == _selectedPackId);
		}

		bool operator!=(const UGsUIWindowSeal::Parameters& InRhs) const
		{
			return !(*this == InRhs);
		}
	};

private:
	enum class EGsSealMode : int32
	{
		SealPack,
		SealNode,
		SealSlot,
	};

	/** 이 윈도우 위젯의 이름 */
	static const FName WINDOW_ASSET_NAME;
	
	/************************************************************************/
	/* 프러퍼티																*/
	/************************************************************************/

protected:
	/** 메인탭버튼으로 사용할 위젯 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCategoryBtn;

	/** 카테고리 버튼 슬롯 헬퍼 */
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _categoryBtnSlotHelper;

	/** 카테고리 토글 그룹 */
	FGsToggleGroup _toggleGroupCategoryTabBtns;
	
	/** 봉인 카테고리 스크롤 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollCategoryBtn;

	/** 봉인 연출용 위젯 스위처 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _sealPackSwitcher;

	/** 봉인 연출용 위젯 참조 배열 */
	TArray<TWeakObjectPtr<UGsUISealPack>> _sealPackWidgetList;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI")
	UGsUISequencePlayer* _sequencePlayer;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
	UWidgetAnimation* _rerollAni;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
	UWidgetAnimation* _rerollSkipAni;

protected:
	/** 뒤로가기 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	/** 닫기 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	/** 메인탭 스위처 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _mainTabSwitcher;

	/** 봉인획득표 */
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _sealPackNameTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sealPackGain0TabBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sealPackGain1TabBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sealPackGain2TabBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _sealPackGainStatListView;

	/** 서브탭버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _subTab0Btn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _subTab0BtnSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _subTab1Btn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _subTab1BtnSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _subTab2Btn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _subTab2BtnSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _subTabSwitcher;

	/** 타이틀 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titlePrefixTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleSuffixTxt;
	
	/** 서브탭 - 봉인노드 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _levelStatListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _expProgressbar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	URichTextBlock* _expDescRichTxt;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _levelDescSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _levelDescTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _showStatBtn;

	/** 서브탭 - 봉인슬롯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlotListEntry* _sealSlotListEntry0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlotListEntry* _sealSlotListEntry1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlotListEntry* _sealSlotListEntry2;
	
	/** 비용 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollEnchantMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfEnchantMaterial;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _enchantMaterialSlotHelper;

	/** 하단바 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _skipToggleBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _skipToggleSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _tryBtn;

	/** 현재 봉인팩의 획득표 데이터 */
	UPROPERTY(Transient)
	TArray<UGsUISealPackGainListItem*> _sealPackGainListItems;
	
	/** 봉인노드의 모든 획득 가능 스탯 목록 */
	UPROPERTY(Transient)
	TArray<UGsUISealLevelStatListItem*> _sealLevelStatListItems;

	/** 봉인노드의 모든 슬롯 목록 */
	UPROPERTY(Transient)
	TArray<UGsUISealSlotListItem*> _sealSlotListItems;

private:
	Parameters _parameters;

	FGsToggleGroup _sealPackGainToggleGroup;
	FGsToggleGroup _subTabToggleGroup;

	/** UI 에 출력중인 봉인슬롯목록의 데이터 */
	TArray<TWeakObjectPtr<UGsUISealSlotListEntry>> _uiSlotListEntries;

	/** UI 에 출력할 아이템 비용 정보. 선택한 서브탭에 따라 레벨업 혹은 리롤 비용을 나타냄. */
	TSharedPtr<FGsCostPackage> _itemCostPackage;

	/** UI 에 출력할 재화 비용 정보. 선택한 서브탭에 따라 레벨업 혹은 리롤 비용을 나타냄. */
	TSharedPtr<FGsCostPackage> _currencyCostPackage;

	/** 봉인팩색상표의 현재 선택한 탭버튼 인덱스 */
	int32 _selectedSealPackGainTabIndex = 0;

	/** 방금 업데이트된 봉인팩의 ID */
	SealPackId _sealPackIdJustUpdated = INVALID_SEAL_PACK_ID;

	/** 방금 업데이트된 봉인노드의 ID */
	SealId _sealNodeIdJustUpdated = INVALID_SEAL_ID;

	/** 신규 레벨이 오픈되었는지 체크하는 용도의 변수로, 현재 선택한 봉인 노드의 마지막 레벨을 저장해 둠. */
	Level _sealNodeLevelLast = 0;

	FGsOnUIEvent _onUiEvent;

	/** 현재 각인 슬롯 리롤 잠금을 할 수 있는지 */
	bool _canChangeRerollLockState = true;


	/************************************************************************/
	/* overrides															*/
	/************************************************************************/

protected:
	void NativeOnInitialized() final;
	void NativeConstruct() final;
	bool SyncContentState() final;
	void NativeDestruct() final;

	/************************************************************************/
	/* behavior                                                             */
	/************************************************************************/

public:
	/** 윈도우를 오픈시킴. 매개변수는 caller 체크용. */
	static TWeakObjectPtr<UGsUIWindowSeal> OpenWindow(const FGsContentsGameBase* InContentsGameBase);

private:
	void ResetNodeSelections();
	void ResetTabs();
	void ResetRerollUnlockSelection();
	void InvalidateCategoryTab();
	void InvalidateSealPack();
	void InvalidateTab(const bool InWithAnimation = false);
	void InvalidateSealPackStatus();
	void InvalidateLevelInfo(const bool InWithAnimation = false);
	void InvalidateSealSlotListArea();
	void InvalidateSealSlotListEntry(const int32 InSlotIndex, const bool InClearEffect);
	void InvalidateCost();
	void InvalidateItemCost();
	void InvalidateCurrencyCost();

private:
	/** 특정 봉인 노드의 모든 레벨별 획득 스탯을 목록으로 입력 */
	void UpdateSealLevelStatListItems(const FGsSealNode& InSealNode, const bool InWithAnimation);

	void UpdateSealSlotListItem(const int32 InSlotIndex);

	/** 레벨업 시도 */
	void TryToLevelup() const;

	/** 기본 체크 후 리롤 시도 */
	void TryToRerollFirst();

	/** 위험 팝업 등 필요한 경우 출력 후 리롤 시도 */
	static void TryToRerollLast(const SealPackId InSealPackId, const SealId InSealNodeId, TSet<SealSlotId> InLockStatSealSlotIds, TSet<SealSlotId> InLockColorSealSlotIds);

	/************************************************************************/
	/* getter																*/
	/************************************************************************/

private:
	/** 현재 선택한 봉인팩 반환. */
	TWeakPtr<const FGsSealPack> GetSelectedPack() const;

	/** Index 에 맞는 봉인팩 정보 반환 */
	TWeakPtr<const FGsSealPack> GetPackWithIndex(const uint8 InIndex) const;

	UGsUISealPack* GetSelectedPackWidget();

	const UGsUISealPack* GetSelectedPackWidget() const;

	/** 현재 선택한 봉인노드 반환. */
	const FGsSealNode* GetSelectedNode() const;

	/** 현재 잠김 슬롯 갯수 반환 */
	int32 GetCurrentRerollLockCount() const;

	/** 현재 선택한 디스플레이 모드 */
	EGsSealMode GetSelectedSealMode() const;

	/** 스킵 활성화 여부를 반환 */
	bool IsEnableSkip() const;

	/** 봉인노드를 가져옴 */
	const FGsSealNode* GetNodeAt(const int32 InIndex) const;

	/** 봉인슬롯을 가져옴 */
	TWeakPtr<const FGsSealSlot> GetSlotAt(const int32 InNodeIndex, const int32 InSlotIndex) const;

	/************************************************************************/
	/* setter																*/
	/************************************************************************/

public:
	/** 파라미터 설정 */
	void SetParameters(Parameters InParameters, const bool InResetCategory = false, const bool InAddCurrentHistory = true);

	/** 현재 특정 슬롯의 잠김 상태를 변경 */
	void SetRerollUnlockTargetUnlocked(const int32 InSlotIndex, const EGsSealRerollUnlockTarget InUnlockTarget, const bool InUnlocked);

	/************************************************************************/
	/* 이벤트 - 그 외													    */
	/************************************************************************/

public:
	/** [패킷] 봉인 레벨업 패킷 수신. InSealNodeId: 실제 레벨업된 노드의 ID */
	void OnSealLevelUp(const SealPackId InSealPackId, const SealId InSealNodeId);

	/** [패킷] 봉인 리롤 패킷 수신 */
	void OnSealSlotChangeAll(const SealPackId InSealPackId, const SealId InSealNodeId);

	/** [패킷] 봉인 슬롯 잠금 상태 변경되었음을 수신 */
	void OnSealSlotLockSave();

	void OnFinishedSealSlotChangeAllAni(const UWidgetAnimation* InAnimation);

	/** 재화 추가/변경/삭제 발생 시 호출됨. */
	void OnChangeCurrency(const CurrencyType InCurrencyType);

	/** 아이템 추가/변경/삭제 발생 시 호출됨. */
	void OnChangeItem(const ItemId InItemId);
	
	/** 레벨 업 등으로 카테고리 언락 변동 발생 시 호출됨 */
	void OnUnlockCategory();

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnUIEvent(const EGsUISealWindowEvent InUIEvent);

	UFUNCTION()
	void OnSelectChangedNode();

	UFUNCTION()
	void OnSelectChangedSealPackGainTabBtn(int32 InIndex);

	UFUNCTION()
	void OnSelectChangedSubTabBtn(int32 InIndex);

	void OnFinishedLongPressEventMaterialItem(UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnRefreshEntryEnchantMaterialList(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickShowSealSlotGainBtn();

	UFUNCTION()
	void OnClickSkipButton();

	UFUNCTION()
	void OnClickTryButton();

	/** 봉인 카테고리 탭 갱신 */
	UFUNCTION()
	void OnRefreshEntryCategoryTabBtn(int32 InIndex, UWidget* InEntry);

	/** 봉인 카테고리 탭 클릭 시 */
	UFUNCTION()
	void OnSelectChangedCategoryTabBtn(int32 InIndex);

	/**
	 * 슬롯 잠금 버튼 클릭 시 발생함. 현재 상태를 매개변수로 받으며, 반환값으로 Unlock 상태를 변경함.
	 */
	void OnClickSealRerollUnlockTarget(const int32 InIndex, const EGsSealRerollUnlockTarget InUnlockTarget);

	void OnArrivedFullProgressOnAnimation();

};
