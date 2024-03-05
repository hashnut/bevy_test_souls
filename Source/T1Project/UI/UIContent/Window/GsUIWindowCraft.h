#pragma once

#include "Runtime/Core/Public/Containers/CircularQueue.h"

#include "Craft/GsUICraftExpectedListItem.h"
#include "Item/Craft/GsCraftTab.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Message/GsMessageContents.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "UI/UIContent/Common/Numpad/TGsCalculateHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UILib/Base/GsUIWindow.h"

class FGsContentsGameBase;
class FGsCostPackage;
class FGsGameStateCraft;
class FGsItem;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsUICraftTargetListItem;
class UGsUIVFX;
class UImage;
class UListView;
class UPanelWidget;
class UProgressBar;
class UScrollBox;
class UTextBlock;
class UWidget;
class UWidgetSwitcher;
struct FGeometry;
class UGsPoolUObject;

#include "GsUIWindowCraft.generated.h"

/**
 * 제작 윈도우
 */
UCLASS()
class T1PROJECT_API UGsUIWindowCraft final : public UGsUIWindow, public IGsTutorialInteractionInterface
{
	
	GENERATED_BODY()

public:
	/** 초기화 파라미터 */
	struct Parameters final
	{

		CraftTabId _mainTabId;

		CraftTabId _subTabId;

		CraftTabId _sub2ndTabId;

		CraftId _craftId;

		Parameters()
		{
			Reset();
		}

		/** 탭 지정이나 제작ID를 지정하여 초기화 */
		Parameters(const CraftTabId InMainTabId, const CraftTabId InSubTabId, const CraftTabId InSub2ndTabId, const CraftId InCraftId)
			: _mainTabId(InMainTabId)
			, _subTabId(InSubTabId)
			, _sub2ndTabId(InSub2ndTabId)
			, _craftId(InCraftId)
		{
		}

		/** 탭경로만 지정하여 초기화 */
		Parameters(const CraftTabId InMainTabId, const CraftTabId InSubTabId, const CraftTabId InSub2ndTabId)
			: Parameters(InMainTabId, InSubTabId, InSub2ndTabId, INVALID_CRAFT_ID)
		{
		}

		/** 제작ID만 지정하여 초기화 */
		explicit Parameters(const CraftId InCraftId)
			: Parameters(INVALID_CRAFT_TAB_ID, INVALID_CRAFT_TAB_ID, INVALID_CRAFT_TAB_ID, InCraftId)
		{
		}

		bool operator==(const UGsUIWindowCraft::Parameters& InRhs) const
		{
			return (InRhs._craftId == _craftId)
				|| (InRhs._mainTabId == _mainTabId && InRhs._subTabId == _subTabId && InRhs._sub2ndTabId == _sub2ndTabId) && (InRhs._craftId == _craftId);
		}

		bool operator!=(const UGsUIWindowCraft::Parameters& InRhs) const
		{
			return !(*this == InRhs);
		}

		bool HasValidTabs() const
		{
			return (INVALID_CRAFT_TAB_ID != _mainTabId) && (INVALID_CRAFT_TAB_ID != _subTabId);
		}

		bool HasValidCraftId() const
		{
			return INVALID_CRAFT_ID != _craftId;
		}

		void Reset()
		{
			_mainTabId = INVALID_CRAFT_TAB_ID;
			_subTabId = INVALID_CRAFT_TAB_ID;
			_sub2ndTabId = INVALID_CRAFT_TAB_ID;
			_craftId = INVALID_CRAFT_ID;
		}

	};

private:
	/** 제작 시 연출 출력을 위해 간단하게 사용할 연출 컨트롤러 */
	class FGsSimpleScenePlayer
	{

		bool _finished;

		TArray<TWeakObjectPtr<UWidget>> _widgets;

		FSimpleDelegate _inOnFinishDelegate;

	public:
		void Add(UWidget* InWidget);

		void Start(FSimpleDelegate InOnFinishDelegate);

		void Rewind();

		void Finish();

		bool IsFinished() const;

	};

protected:
	/** 이 윈도우 위젯의 이름 */
	static const FName WINDOW_ASSET_NAME;

	/** 계산기에 입력 가능한 최소값 */
	static const int32 CALCULATE_HELPER_MIN_VALUE;

	/** 탭경로 생성 시 유효한 것으로 간주할 최소 탭경로 뎁스 */
	static const int32 TAB_ROUTE_MIN_DEPTH;

	/************************************************************************/
	/* 프러퍼티																*/
	/************************************************************************/

protected:
	/** 메인탭버튼으로 사용할 위젯 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfMainTabBtn;

	/** 서브탭버튼으로 사용할 위젯 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfSubTabBtn;

	/** 서브2탭버튼으로 사용할 위젯 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfSub2ndTabBtn;

	/** 제작목록으로 사용할 위젯 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCraftTargetBtn;

	/** 재료목록으로 사용할 위젯 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCraftMaterialBtn;
	
	/** 제작 버튼 클릭 후, 연출 시간(초). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	float _spellingDurationSecs = 1.f;

	/** 연출 시작 시간 */
	FDateTime _spellingStartTime;

	/** 연출 컨트롤러 */
	FGsSimpleScenePlayer _scenePlayer;

	/************************************************************************/
	/* 상단 바																*/
	/************************************************************************/

protected:
	/** 뒤로가기 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	/** 닫기 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	
	/************************************************************************/
	/* 네비게이션														    */
	/************************************************************************/

protected:
	/** 이전 제작으로 이동 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCraftBack;

	/** 메인탭버튼 스크롤 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _scrollMainTabBtns;

	/** 서브탭버튼 스크롤 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollSubTabBtns;
	
	/** 서브2탭버튼 스크롤 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollSub2ndTabBtns;

	/** 서브2탭버튼 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelSub2ndTabBtns;
	
	/** 제작대상 리스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _craftTargetList;

	/** "즐겨찾기 없음" 위젯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelEmptyFavoriteHelp;

	/** 메인탭버튼 슬롯헬퍼 */
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperMainTabBtns;

	/** 서브탭버튼 슬롯헬퍼 */
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSubTabBtns;

	/** 서브2탭버튼 슬롯헬퍼 */
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSub2ndTabBtns;

	FGsToggleGroup _toggleGroupMainTabBtns;

	FGsToggleGroup _toggleGroupSubTabBtns;

	FGsToggleGroup _toggleGroupSub2ndTabBtns;

	TArray<const FGsCraftTab*> _cachedCraftMainTabs;

	TArray<const FGsCraftTab*> _cachedCraftSubTabs;

	TArray<const FGsCraftTab*> _cachedCraftSub2ndTabs;

	TArray<UGsUICraftTargetListItem*> _craftTargetListItems;

	/** 제작 제한이 걸려 있을 경우, EndTime 뒤에 붙을 접미사 (예를 들어 ' 까지') */
	FString _endTimeSuffix;

	/**
	 * 현재 선택된 아이템제작의 비용패키지.
	 * 이 인스턴스를 일부러 복사할 필요는 없으나, "재료의 우선순위" 변경 시,
	 * 리셋 타이밍이 "다른 아이템제작 선택시" 로 설정되어 복제한 비용패키지를 사용함.
	 * 또한, 이 값은 "뒤로가기" 를 지원하기 위해 백업이 필요함.
	 */
	TSharedPtr<FGsCostPackage> _copyedCostPackage;

	UPROPERTY(Transient)
	UGsPoolUObject* _craftTargetListItemsPool;

	/************************************************************************/
	/* 상세정보																*/
	/************************************************************************/

protected:
	/** 상세정보/선택없음 스위처 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetail;
	
	/** "선택없음" 의 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtEmptySelectionHelp;

	/** 상세정보 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _boxDetail;

	/** 상세정보/예상목록 스위처 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _boxDetailSwitcher;

	/** 상세정보/예상목록 전환 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetailSwitcher;

	/** 제작 갯수 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _boxCraftAmount;

	/** 제작 이름 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtCraftName;

	/** 제작 실패 안내 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _containerFailGuide;
	
	/** 제작 대상 아이콘 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCraftTargetIcon;

	/** 제작 대상 아이콘 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgCraftTargetIcon;

	/** 제작 시 아이콘에 출력할 연출 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxOnCraftIcon;

	/** 성공 확율 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtSuccessRatio;
	
	/** 서버 제한 갯수 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _containerLimitCountServer;

	/** 서버 제한 갯수 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtLimitCountServer;
	
	/** 계정 제한 갯수 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _containerLimitCountAccountByServer;

	/** 계정 제한 갯수 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtLimitCountAccountByServer;

	/** 초기화 안내 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _containerReset;

	/** 초기화 안내 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtReset;

	/** 만료 안내 컨테이너 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _containerExpire;

	/** 만료 안내 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtExpire;

	
	/** 제작 예상 목록0 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICraftExpectedListItem* _craftExpectedListItem0;
	
	/** 제작 예상 목록1 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICraftExpectedListItem* _craftExpectedListItem1;
	
	/** 제작 예상 목록2 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICraftExpectedListItem* _craftExpectedListItem2;

	/** 성공 확율 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtCraftExpectedSuccessRatio;

	TArray<UGsUICraftExpectedListItem*> _craftExpectedListItems;

	TArray<UGsUICraftExpectedListItem::Parameters> _craftExpectedListItemParameters;
	
	/************************************************************************/
	/* 제작 재료															*/
	/************************************************************************/

protected:
	/** 제작 재료 스크롤 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollCraftMaterialBtns;

	/** 제작 재료 슬롯 헬퍼 */
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCraftMaterialBtns;

	FGsToggleGroup _toggleGroupCraftMaterialBtns;

	/************************************************************************/
	/* 하단																    */
	/************************************************************************/

protected:
	/** 하단 버튼 스위처 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBottom;

	/** 계산기 리셋 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcReset;
	
	/** 계산기 +10 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcPlus10;
	
	/** 계산기 최대 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcMax;
	
	/** 계산기 -1 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcMinus1;

	/** 계산기 현재값 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtCalcCurrent;
	
	/** 계산기 +1 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcPlus1;
	
	/** 계산기 팝업 오픈 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcOpen;

	/** 제작시도 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCraft;

	TGsCalculateHelper<int32> _calculateHelper;

	/** 계산기 팝업을 닫을 때, 애니메이션 출력 등으로 인해 닫음 상태를 즉시 체크할 수 없으므로, 오픈 상태를 별도로 저장하여 사용함 */
	bool _isOpeningPopupCalculator;

	/** 제작 목록 클릭 시, '결과 화면' 을 보여주는 플레이어의 선택이 유지되도록 설정 */
	uint8 _isBoxDetailResult;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressCraft;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCraftCancel;

	/************************************************************************/
	/* overrides															*/
	/************************************************************************/

protected:
	void NativeOnInitialized() override;

	void NativeConstruct() override;

	bool SyncContentState() override;

	void NativeDestruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/************************************************************************/
	/* invalidate															*/
	/************************************************************************/

public:
	/** 제작목록을 새로고침. */
	void InvalidateCraftTargetListEntries(const bool InResetFirst = false);

private:
	void InvalidateUIInterationState();

	void InvalidateMainTabBtns();

	void InvalidateSubTabBtns(const bool InResetScroll = false);

	void InvalidateSub2ndTabBtns();

	void InvalidateCraftTargetListEntry(const int32 InIndex);

	void InvalidateCraftTargetDetail();

	void InvalidateSpelling();

	/**
	 * 다른 리스트와 다르게, 재료 버튼 목록은 상황에 따라 새로고침만 하거나 리셋 후 새로고침이 필요할 수 있어 함수로 분리함.
	 */
	void ResetCraftMaterialBtns();

	void DeselectCraftMaterialBtns();
	
	void InvalidateCraftMaterialBtns();

	void InvalidateCalculateHelper();

	/************************************************************************/
	/* behavior                                                             */
	/************************************************************************/

public:
	/** 윈도우를 오픈시킴. 매개변수는 caller 체크용. */
	static TWeakObjectPtr<UGsUIWindowCraft> OpenWindow(const FGsContentsGameBase* InContentsGameBase);

	/** 스펠링 효과를 종료시키고, 실제 제작을 시도함 */
	void EndSpelling(const CraftId InCraftTargetId, const int32 InCraftCount);

private:
	void ResetAllDatas();

	/** 현재 설정 상태에서 제작 가능한지 체크 후 불가능할 경우, 티커 출력 등 수행. 제작 가능 여부를 반환. */
	bool ValidateCanCraftNow() const;

	/**
	 * 현재 창의 상태를 히스토리에 추가
	 */
	void AddToHistoryQueueThis();

	void OpenPopupCalculator();

	void OpenPopupSortCandidateGoods(const uint8 InIndex);

	/************************************************************************/
	/* setter																*/
	/************************************************************************/

public:
	/** 특정 제작대상이 업데이트되면 이 함수를 호출하여, 출력을 새로고침함 */
	void CraftTargetUpdated(const CraftId InCraftId);

	/** 제작창을 설정 */
	void SetParameters(const Parameters& InParameters, const bool InAddCurrentHistory = true);

private:
	void SetParametersPrivate(const Parameters& InParameters);

	void SelectMainTabBtn(const CraftTabId InCraftTabId);

	void SelectSubTabBtn(const CraftTabId InCraftTabId);

	void SetVisibilitySub2ndTabBtns(const bool InVisible);

	void ToggleVisibilitySub2ndTabBtns();

	void SelectSub2ndTabBtn(const CraftTabId InCraftTabId);

	void SelectCraftTargetBtn(const CraftId InCraftTargetId);

	void ResetCraftTargetCostPackage();

	void NavigateTo(const CraftTabId InMainTabId, const CraftTabId InSubTabId, const CraftTabId InSub2ndTabId, const CraftId InCraftId = INVALID_CRAFT_ID);

	void SetSpellingState(const bool InIsSpelling);

	/************************************************************************/
	/* getter																*/
	/************************************************************************/

private:
	static FGsGameStateCraft* GetGameStateCraft();

	static void MakeDisplayableChildTabs(const FGsCraftTab* InParentTab, OUT TArray<const FGsCraftTab*>& OutCraftTabs);

	const FGsCraftTab* GetSelectedMainTab() const;

	const FGsCraftTab* GetSelectedSubTab() const;

	const FGsCraftTab* GetSelectedSub2ndTab() const;

	const FGsCraftTab* GetSelectedTabTail() const;

	const FGsCraftTarget* GetSelectedCraftTarget() const;

	TSharedPtr<FGsCostPackage> GetSelectedCraftTargetCostPackage();

	int32 GetCalcCurrentValue() const;

	bool IsSpellingState() const;

	/************************************************************************/
	/* 이벤트 - 탭버튼													    */
	/************************************************************************/

protected:
	UFUNCTION()
	void OnRefreshEntryMainTabBtn(int32 InIndex, UWidget* InEntry);

	void OnSelectChangedMainTabBtn(int32 InIndex);

	UFUNCTION()
	void OnRefreshEntrySubTabBtn(int32 InIndex, UWidget* InEntry);

	void OnSlotClickedSubTabBtn(int32 InIndex, bool bIsSelected);

	void OnSelectChangedSubTabBtn(int32 InIndex);

	UFUNCTION()
	void OnRefreshEntrySub2ndTabBtn(int32 InIndex, UWidget* InEntry);

	void OnSlotClickedSub2ndTabBtn(int32 InIndex, bool bIsSelected);

	void OnSelectChangedSub2ndTabBtn(int32 InIndex);

	/************************************************************************/
	/* 이벤트 - craft target												*/
	/************************************************************************/

protected:
	void UpdateCraftTargetListItems(const bool InResetFirst = false);

	static void UpdateCraftTargetListItem(UGsUICraftTargetListItem& InListItem);

	void OnCraftTargetListItemSelectionChanged(UObject* InListItem);

	UFUNCTION()
	void OnClickCraftTargetListItem(UObject* InListItem);

	UFUNCTION()
	void OnClickedDetailSwitcher();

	UFUNCTION()
	void OnClickedCraftTargetIcon();
	
	void OnRefreshEntryCraftExpectedListItem(int32 InIndex, UWidget* InEntry);
	
	UFUNCTION()
	void OnRefreshEntryCraftMaterialBtn(int32 InIndex, UWidget* InEntry);

	void OnClickedCraftMaterialCraft(const int32 InIndex);

	void OnClickedCraftMaterialShowCandidate(const int32 InIndex);

	/************************************************************************/
	/* 이벤트 - 하단													    */
	/************************************************************************/

protected:
	UFUNCTION()
	void OnChangedCalculateHelper(int32 InNumber);

	UFUNCTION()
	void OnClickedCalcReset();

	UFUNCTION()
	void OnClickedCalcPlus10();

	UFUNCTION()
	void OnClickedCalcMax();

	UFUNCTION()
	void OnClickedCalcMinus1();

	UFUNCTION()
	void OnClickedCalcPlus1();

	UFUNCTION()
	void OnClickedCalcOpen();

	UFUNCTION()
	void OnClickedCraft();

	UFUNCTION()
	void OnClickedCraftCancel();

	void OnFinishCalculatePopupCalculator(int64 InValue);

	void OnNativeDestructPopupCalculator();

	/************************************************************************/
	/* 이벤트 - 그 외													    */
	/************************************************************************/
	
public:
	/** 제작 결과 패킷을 받음. */
	void OnCraftResult(const bool InIsSuccess);

	/** 재화 추가/변경/삭제 발생 시 호출됨. */
	void OnChangeCurrency(const CurrencyType InCurrencyType);

	/** 아이템 추가/변경/삭제 발생 시 호출됨. */
	void OnChangeItem(const ItemId InItemId);

protected:
	// 제작 뒤로가기 버튼 클릭 시
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCraftBack();

	/** 대체아이템 순서 선택 팝업에서 OK 클릭 시 발생 */
	void OnOkPopupSortCandidateGoods();

	//------------------------------------------------------------------------------------------------------------------
	// 튜토리얼
private:
	bool _bIsTutorial = false;
	bool _bIsTutorialInteraction = false;

	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) override;
	virtual bool IsTutorialInteraction() const override { return _bIsTutorialInteraction; }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() override { return OnTutorialInteraction; }
	virtual void OnClickTutorialRegion() override;

public:
	UWidget* SetTutorial(bool bIsTutorial);
};