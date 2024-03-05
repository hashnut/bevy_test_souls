#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUITray.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "Summon/GsSummonDefine.h"

#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageUI.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUITraySummon.generated.h"

class UGsButton;
class UGsUICurrencyButton;
class UGsDynamicPanelSlotHelper;
class UGsUIItemButton;
class UGsUISummonSkipToggleButton;
class UGsUISummonCardItem;
class UGsHorizontalBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconBase;
class UGsUIIconItem;
class UGsUIProgressBar;
class UGsUISummonMileageAddedEffect;

class UWidgetSwitcher;
class UUserWidget;
class UHorizontalBox;
class UWidget;
class UImage;
class UCanvasPanel;
class UWidgetAnimation;

class FGsSummonHandler;
class FGsSummonCardData;

struct IGsMessageParam;

UCLASS()
class T1PROJECT_API UGsUITraySummon : public UGsUITray
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _horizontalBox;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;
	// 모두 열기 버튼
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _btnOpenAll;
	// 닫기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _btnClose;
	// 닫기 2(다시뽑기 옆에꺼)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _btnClose2;
	// 닫기 3(다시뽑기 아이템 옆에꺼)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _btnClose3;
	// 다시 뽑기(retry)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsUICurrencyButton* _btnRetry;
	// 결과 보기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _btnShowResult;
	// 버튼 타입
	// 0: hide
	// 1: 모두 열기	
	// 2: 결과 바로 보기
	// 3: 나가기
	// 4: 다시 뽑기
	// 5: 다시 뽑기(아이템)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBtnType;

	
	// 다시뽑기(아이템)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsUIItemButton* _btnRetryItem;
	// 토글 스킵 버튼
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsUISummonSkipToggleButton* _toggleSkipButton;

	// all card(11)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsUISummonCardItem*> _arrayCardItem;
	// use card
	TArray<UGsUISummonCardItem*> _arrayNowUseCardItem;

	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animation6Card;

	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animation11Card;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* _cardPanel;

	// switcher bg
	// 0: fairy, 1: costume
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBG;


	// 뽑기 마일리지 아이템 셀렉터(보상)
	//UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	//UGsHorizontalBoxIconSelector* _targetSummonMileageItemIconSelector;
	// 뽑기 보상 아이콘 생성하는거
	//UPROPERTY()
	//UGsDynamicIconSlotHelper* _targetSummonMileageIconSlotHelper;

	// 뽑기 마일리지 프로그래스 바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _summonMileageProgressBar;

	// 마일리지 제목(페어리, 코스튬)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMileageTitle;

	// 마일리지 보상 정보(획득 불가, 획득 가능 스텝)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMileageRewardInfo;
	// visible 처리용 widget(아이템 뽑기면 show, 나머진 hide)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _mileageVisibleWidget;
	// 마일리지 이펙트 위젯
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _mileageRewardEffectWidget;
	// 마일리지 보상 추가 이펙트(애니 트는용)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISummonMileageAddedEffect* _mileageAddedEffect;


	// 소환 마일리지 팝업 열기 버튼
	// https://jira.com2us.com/jira/browse/CHR-24962
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOpenPopupSummonMileage;
	// 소환 마일리지 팝업 열기 버튼 스위처
	// 0: 코스튬
	// 1: 페어리
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBtnSummonMileage;


	FGsSummonHandler* _summonHandler;
	// 카드 갯수
	int _countCard = 0;
	// 열린 카드 갯수
	int _countOpenCard = 0;

	FTimerHandle _timerHandleTestRetrySummon;

	TArray<int> _currentSlotId;

	bool _totalCardAniStart = false;

	int _lowCardCount = 0;

	// 보상 아이템 tbl id
	int _rewardItemTblId = 0;
	// 보상 아이템 갯수
	int _rewardItemCount = 0;

	TPair<MessageUserInfo, FDelegateHandle> _handlerUserInfo;
	MsgUIHandleArray _uiMsgArray;
	// 마지막 카드까지 열렸는지
	bool _isLastStep = false;

	// 가상 함수
public:
	virtual void BeginDestroy() override;
	// 자동이동 워프전 확인용
	// 소환 tray은 막음
	virtual bool GetPreventAutoClose() const override { return true; }
	
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_TRAY_SUMMON; }

	// 뽑기 마일리지 보상 아이템 툴팁 popup 이 tray에 가려서 안보여서
	// offline result 처럼 처리 해본다
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_POPUP - 1; }
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// bp call function
public:
	// 모두 열기 클릭
	UFUNCTION()
	void OnClickOpenAll();
	// 닫기 클릭
	UFUNCTION()
	void OnClickClose();
	// 결과 보기(고등급용)
	UFUNCTION()
	void OnClickShowResult();
	// 다시 뽑기
	UFUNCTION()
	void OnClickRetry();
	
	UFUNCTION()
	void FinishAnimationBP();

	UFUNCTION()
	void OnClickOpenPopupSummonMileage();

	//UFUNCTION()
	//void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	//UFUNCTION()
	//void OnCreateTargetIcon(UGsUIIconBase* InIcon);

	// 로직 함수
public:
	// 버튼 상태 세팅
	void SetButtonType(EGsSummonBtnType In_type);	
	void CloseTraySummon();	
	// 리스트 초기화
	void InitList();
	// 리스트 삭제
	void ClearList();
	// 반복 데이터 갱신
	void InitRetryData();
	// 마지막 단계 버튼 세팅
	void SetButtonLastStep();
	// 카드 오픈 애니 끝 호출
	void OnFinishCardOpenAni();

	// 스킵할지 선택
	void OnSelectSkip(bool In_isSelect);

	void TestAutoRetrySkip();

	void SetCardItemData(FGsSummonCardData* In_cardData, UGsUISummonCardItem* In_cardItem,
		float In_timeAnyDelay, bool In_isHighCard);

	void StartAnimation();

	void StartCardAnimation(bool In_isSkip = false);


	// open remain card
	void ShowRemainVeilCard();

	void PlayHighGetCardAnimation();

	void SkipAnimation();
	// https://jira.com2us.com/jira/browse/C2URWQ-4063
	// 버튼 상태 갱신
	// 1. 고등급 다열림(QUIT)
	// 2. 하나라도 안열림(SHOW_RESULT)
	void ChangeButtonState();

	void SetSkipButtonVisible(bool In_isVisible);

	//https://jira.com2us.com/jira/browse/C2URWQ-5123
	//고등급만 있을때 두번 터치 해야되는 이슈
	void SetFirstButtonState();

	// 뒷면 카드 이펙트 애니 플레이(플레이 안된것들)
	// 타임라인까지 도달 못하고 스킵되어서 이벤트 호출 안된것들 처리
	void PlayBackCardEffectAnimation();

	// 카드 애니메이션 unbind 처리
	// 빠른 연타로 다시뽑기로 넘어갔는데 카드 애니메이션 콜백이 호출되어 버튼이 보이는 이슈 수정
	// clear 시점에 bind 된 animation callback 정리하기
	// https://jira.com2us.com/jira/browse/C2URWQ-6044
	void UnbindCardAnimationFunction();

	// 메시지 바인딩 함수
public:
	// 재화 갱신 메시지
	void OnUpdateCurrency(uint64 InType);

	// 소환 마일리지 데이터 갱신
	void OnChangedSummonMileageData(const  IGsMessageParam* inParam);

	// get
public:
	EGsSummonBtnType GetButtonType() const;

	UWidgetAnimation* GetNowAnimation();
	// get slot index by data index
	int GetCurrentCardItemSlotIndex(int In_dataIndex);
	// check all card opened(high card: over rare grade)
	bool IsAllCardOpened();
	// 모든 카드가 고등급인지
	bool IsAllCardHigh();
	// 보상 데이터 세팅
	void SetRewardData(int In_rewardItemId, int In_rewardItemCount);
	// 마일리지 제목 텍스트 세팅(코스튬, 페어리)
	void SetMileageTitleText();
	// 마일리지 보상 텍스트 세팅(획득 가능, 불가)
	void SetMileageRewardInfoText(bool In_isRewardOn);
	// 마일리지 데이터 세팅
	void SetMileageData(Currency In_mileage, int In_itemCount, bool In_isProgressAnim = false);
	// 현재 마일리지 데이터로 세팅
	void SetNowMileageData();
	// 최초에 마일리지 관련 위젯 hide 하기
	void SetMileageWidgetHide();
};