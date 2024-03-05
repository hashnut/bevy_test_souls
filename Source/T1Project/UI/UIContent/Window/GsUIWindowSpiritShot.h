// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotSceneTabBase.h"

#include "GsUIWindowSpiritShot.generated.h"

class FGsContentsGameBase;
class FGsSpiritShotCategoryData;
class FGsSpiritShotLevelData;

class FGsCostPackage;
class FGsGameStateSpiritShot;
class FGsItem;
class UGsButton;
class UGsSwitcherButton;
class UGsUICurrencyButton;
class UGsDynamicPanelSlotHelper;
class UGsUIVFX;
class UImage;
class UPanelWidget;
class UScrollBox;
class UTextBlock;
class UWidget;
class UWidgetSwitcher;
class UGsUITooltipDesc;
class UGsUITooltipSpiritShotProb;
class UGsUISpiritShotSceneTabBase;
class UCanvasPanel;



/** SwitcherButton 으로 정의된 확률 증감 버튼 상태 가독성 향상을 위함 */
enum EGsSpiritShotButtonState
{
	Enabled = 0,	// 활성화
	Disabled = 1,	// 비활성화
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowSpiritShot : public UGsUIWindow
{
	GENERATED_BODY()

	
public:
	/** 초기화 파라미터 */
	struct Parameters
	{
		SpiritShotCategoryId _categoryId;
		//SpiritShotLevelId _levelId;

		Parameters() :
			_categoryId(0)
			//, _levelId(0)
		{
		}

		explicit Parameters(SpiritShotCategoryId InCategoryId) :
			_categoryId(InCategoryId)
			//, _levelId(0)
		{
		}

		//explicit Parameters(SpiritShotCategoryId InCategoryId, SpiritShotLevelId InLevelId) :
		//	_categoryId(InCategoryId),
		//	_levelId(InLevelId)
		//{
		//}

		bool operator==(const UGsUIWindowSpiritShot::Parameters& InRhs) const
		{
			return _categoryId == InRhs._categoryId;
		}

		bool operator!=(const UGsUIWindowSpiritShot::Parameters& InRhs) const
		{
			return !(*this == InRhs);
		}
	};


	/************************************************************************/
	/* 상단 바                                                              */
	/************************************************************************/
protected:
	/** 뒤로 가기 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	/** 닫기 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	
	/** 천마석 시스템 가이드 툴팁 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnHelp;


	/************************************************************************/
	/* 카테고리                                                             */
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

	/** 잠식 카테고리 스크롤 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollCategoryBtn;

	/** 천마석 통합 레벨 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTotalLevel;

	/** 통합 잠식 레벨 설명 팝업 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSumLevelInfo;

	/** 현재 카테고리 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtCategory;

	/** 현재 카테고리에서의 레벨 텍스트. ('Lv.' 도 함께 출력) */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtCategoryLevel;


	/************************************************************************/
	/* 연출용 위젯			                                                */
	/************************************************************************/
protected:
	/** 천마석 연출용 위젯 스위처 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _sceneTabSwitcher;

	/** 천마석 연출용 위젯 참조 배열 */
	TArray<TWeakObjectPtr<UGsUISpiritShotSceneTabBase>> _sceneTabList;

	/** 연출 스킵 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnSkipPrev;
	

	/************************************************************************/
	/* 잠식 확률 영역                                                       */
	/************************************************************************/
protected:
	/** 잠식 확률 설명 툴팁 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEnchantProbInfo;

	///** 잠식 확률 설명 툴팁 */
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUITooltipDesc* _tooltipEnchantProbInfo;

	/** 잠식 확률 툴팁 (설명 및 확률도 표시해 주어야 함) */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipSpiritShotProb* _tooltipEnchantProb;

	/** 잠식 하락 가능할 경우 경고로 켜줄 패널 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _fallAlertPanel;

	/** 잠식 확률 영역 스위쳐 (최대 레벨 도달 시, 전용 UI 로 가려주어야 함) */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherProb;

	/** 강화확률 올리는 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnAddProb;

	/** 강화확률 내리는 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnSubProb;

	/** 강화확률 최대치로 올리는 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnMaxProb;

	/** 강화확률 최소치로 내리는 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnMinProb;

	/** 강화확률 현재값 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtProb;

	/** 잠식 재료 스크롤 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollEnchantMaterial;

	/** 재료 목록으로 사용할 위젯 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfEnchantMaterial;

	/** 잠식 재료 슬롯 헬퍼 */
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _enchantMaterialSlotHelper;

	/** 잠식 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnEnchant;


	/************************************************************************/
	/* 잠식 효과 영역                                                       */
	/************************************************************************/
protected:
	/** 잠식 효과 설명 툴팁 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEnchantStatInfo;

	/** 잠식 효과 설명 툴팁 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _enchantStatInfoTooltip;

	/** 현재 레벨 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtCurrentLevel;

	/** 다음 레벨 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtNextLevel;

	/** 잠식 현재 효과 0렙용 패널 (0렙 예외처리 대응) */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelNoCurrentStat;

	/** 잠식 현재 효과 스크롤 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollCurrentStat;

	/** 잠식 현재 효과 스탯 설명 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtCurrentLevelStat;

	/** 잠식 다음 효과 만렙용 패널 (만렙 예외처리 대응) */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelNoNextStat;

	/** 잠식 다음 효과 스크롤 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollNextStat;

	/** 잠식 다음 효과 스탯 설명 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtNextLevelStat;

	/**
	 * 잠식 효과 보여주는 방식을 UGsDynamicPanelSlotHelper 을 이용한 리스트로 표현하다가, 
	 * 현재는 UTextBlock 에 한꺼번에 대입하는 방식으로 변경 
	 */
	///** 잠식 효과 보여주기 위해 사용할 위젯 */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	//TSubclassOf<UUserWidget> _subClassOfEnchantStat;

	///** 잠식 현재 효과 슬롯 헬퍼 */
	//UPROPERTY()
	//UGsDynamicPanelSlotHelper* _enchantCurrentStatSlotHelper;

	///** 잠식 현재 효과 슬롯 헬퍼 */
	//UPROPERTY()
	//UGsDynamicPanelSlotHelper* _enchantNextStatSlotHelper;

	/** 카테고리 탭 캐싱 (잠식 도중에 카테고리 탭에 대한 Input Action 을 차단하기 위해) */
	TArray<TWeakObjectPtr<UWidget>> _categoryTabList;


	/************************************************************************/
	/* Data                                                                 */
	/************************************************************************/
private:
	/** 윈도우에서 사용할 파라미터 (카테고리 id 를 담고 있음) */
	Parameters _paramters;

	/** 현재 윈도우에서 선택된 카테고리가 바라보고 있는 레벨 Id 값 (레벨 변화 감지) */
	SpiritShotCategoryId _currentLevelId;

	/** 확률 조절 횟수 (기본값은 0) */
	int32 _rateModifyCount;

	/** 현재 강화를 위해 필요한 재료 CostPackage. 골드는 따로 계산한다 */
	TSharedPtr<FGsCostPackage> _costPackage;

	/** 이 윈도우 위젯의 이름 */
	static const FName WINDOW_ASSET_NAME;

	/** 확률 툴팁 텍스트 */
	FText _enchantProbTooltipText;

	/** 퍼센트 기호 텍스트 : {0}% */
	FText _enchantProbSign;

	/** 레벨 효과 툴팁 텍스트 */
	FText _enchantStatTooltipText;

	/** 연출 스킵 여부 */
	bool _isSkipPrevAni;


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
private:
	/** 확률 조절값 등, 기본 데이터 초기화 */
	void InitializeData();

	/** 천마석 연출 위젯 상태 변경 */
	void ChangeSceneTabState(EGsStateSpiritShotSceneState InSceneState, EGsStateSpiritShotLevel InLevelState = EGsStateSpiritShotLevel::LEVEL_SAME, TArray<ItemDBIdAmountPair> InMaterialInfo = TArray<ItemDBIdAmountPair>());

	/** 강화 연출 재생 중에 잠식 버튼과 카테고리 영역이 인풋 액션을 받지 않도록 설정 */
	void BlockInputIfEnchantInProgress(bool InBlockInput);


public:
	/** 윈도우를 오픈시킴. 매개변수는 caller 체크용. */
	static TWeakObjectPtr<UGsUIWindowSpiritShot> OpenWindow(const FGsContentsGameBase* InContentsGameBase);


	/************************************************************************/
	/* UI 갱신 관련 : 각각의 UI 업데이트는 독립적으로 작동해야 함           */
	/************************************************************************/
private:
	/** 탭 업데이트 (언락 등 이벤트에 대응) */
	void InvalidateCategoryTab();

	/** 현재 카테고리에 해당하는 텍스트 데이터 업데이트 */
	void InvalidateCategoryData();

	/** 강화 확률 업데이트 (확률값 및 증감 버튼) */
	void InvalidateEnchantProbability();

	/** 강화 재료 갯수 업데이트 */
	void InvalidateCost();

	/** 재화 및 강화 버튼 업데이트 */
	void InvalidateCurrency();

	/** 잠식 효과 업데이트 (나중에 팝업이 열려 있다면, 팝업도 업데이트해야 함)*/
	void InvalidateEnchantStat();

	// SceneTab 을 따로 두는 것으로 대체
	///** 잠식 카테고리별 이미지 업데이트 */
	//void InvalidateCoreImage();

	///** 잠식 레벨별 이미지 업데이트 */
	//void InvalidateStepImage();

	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
private:
	/** 현재 선택한 카테고리의 데이터 반환 */
	const FGsSpiritShotCategoryData* GetSelectedCategoryData() const;

	/** 현재 선택한 카테고리의 레벨 데이터 반환 */
	const FGsSpiritShotLevelData* GetSelectedLevelData() const;

	/** 특정 Id 의 레벨 데이터 반환 */
	const FGsSpiritShotLevelData* GetSpecificLevelData(SpiritShotLevelId InLevelId) const;

	/** 특정 인덱스의 카테고리 데이터 반환 */
	const FGsSpiritShotCategoryData* GetCategoryDataAtIndex(int32 InIndex) const;

	/** 천마석 통합 성장 레벨 반환 */
	const int32 GetEnchantSumLevel() const;

	/** 현재 선택한 카테고리가 완료되었는지 여부 */
	bool IsSelectedCategoryCompleted() const;

	/************************************************************************/
	/* Setter                                                               */
	/************************************************************************/
public:
	/**
	 * 파라미터 설정. 윈도우 오픈 시 사용된다 .
	 * 설계 카테고리별로 UI 를 분리했다면 더 깔끔했을 것이다.
	 * 현재는 카테고리를 변경하기 위해 탭을 클릭하면, _parameter 가 바뀌기 때문이다.
	*/
	void SetParameters(Parameters&& InParameters, bool InInternal = false);


	/************************************************************************/
	/* 이벤트 - 외부                                                        */
	/************************************************************************/
public:
	/** [패킷] 천마석 강화 성공/실패 여부 패킷 수신. 실패 보상이 있을 경우 티커 처리 */
	void OnSpiritShotAckEnchant(bool InPacketNoError, SpiritShotLevelId InLevelId, TArray<ItemIdAmountPair> InItemDBIdAmountPairList);

	/** 재화 추가/변경/삭제 발생 시 호출됨. */
	void OnChangeCurrency(const CurrencyType InCurrencyType);

	/** 아이템 추가/변경/삭제 발생 시 호출됨. */
	void OnChangeItem(const ItemId InItemId);

	/** 윈도우 열려 있는 상태에서, 컨텐츠가 언락될 때 호출됨 */
	void OnUnlockCategory();

	/** 강화 연출이 완전히 종료되었을 때 호출됨 */
	void OnFinishedEnchantEffect();

	/************************************************************************/
	/* 이벤트 - 내부                                                        */
	/************************************************************************/
protected:
	/** 천마석 통합 레벨 팝업 버튼 클릭 시 */
	UFUNCTION()
	void OnClickedTotalLevelPopup();

	///** 천마석 카테고리 탭 생성 */
	//UFUNCTION()
	//void OnCreateEntryCategoryTabBtn(UWidget* InEntry);

	/** 천마석 카테고리 탭 갱신 */
	UFUNCTION()
	void OnRefreshEntryCategoryTabBtn(int32 InIndex, UWidget* InEntry);

	/** 천마석 카테고리 탭 클릭 시 */
	UFUNCTION()
	void OnSelectChangedCategoryTabBtn(int32 InIndex);


	/** 잠식 확률 툴팁 버튼 클릭 시 */
	UFUNCTION()
	void OnPressedEnchantProbTooltip();

	/** 잠식 확률 최소화 버튼 클릭 시 */
	UFUNCTION()
	void OnClickedProbMinBtn();

	/** 잠식 확률 감소 버튼 클릭 시 */
	UFUNCTION()
	void OnClickedProbSubBtn();

	/** 잠식 확률 최대화 버튼 클릭 시 */
	UFUNCTION()
	void OnClickedProbMaxBtn();

	/** 잠식 확률 증가 버튼 클릭 시 */
	UFUNCTION()
	void OnClickedProbAddBtn();

	/** 잠식 버튼 클릭 시 */
	UFUNCTION()
	void OnClickedEnchantBtn();

	/** 잠식 재료 목록 갱신 */
	UFUNCTION()
	void OnRefreshEntryEnchantMaterialList(int32 InIndex, UWidget* InEntry);

	/** 잠식 효과 툴팁 버튼 클릭 시 */
	UFUNCTION()
	void OnPressedStatInfoTooltip();

	/** 잠식 연출 스킵 버튼 클릭 시 */
	UFUNCTION()
	void OnClickedSkipPrevAniBtn();

	///** 현재 레벨 효과 목록 갱신 */
	//void OnRefreshEntryCurrentStatList(int32 InIndex, UWidget* InEntry);

	///** 다음 레벨 효과 목록 갱신 */
	//void OnRefreshEntryNextStatList(int32 InIndex, UWidget* InEntry);
};
