#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIWindow.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Message/GsMessageUI.h"

#include "GsUIWindowStarLegacy.generated.h"
/*
* window 별의 유산
*/

class UGsUIStarLegacySlot;
class UGsButton;
class UGsUICurrencyButton;
class UGsUITabButtonCommon;

class UPanelWidget;

struct IGsMessageParam;

struct FGsSchemaStarLegacyCategory;


UCLASS()
class UGsUIWindowStarLegacy : public UGsUIWindow 
{
	GENERATED_BODY()
protected:
	// 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsUIStarLegacySlot*> _arraySlots;

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	// 백 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	// 오픈 남은 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textOpenRemainCount;

	// bm 아이템 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textBMItemCount;

	// 아래 부분(연출시 hide 할 것)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelBottom;
	// 룰렛 진행중(skip 버튼)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRoulette;

	// 소환 시작 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnSummonStart;
	// 슬롯 배치 변경 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnReroll;
	
	// 탭 1성계
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITabButtonCommon* _tabCategory1;

	// 탭 2성계
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITabButtonCommon* _tabCategory2;
private:
	MsgUIHandleArray _uiMsgArray;

	// 현재 틱 시간(n바퀴째 시간)
	float _currentTimeTickWaitingTimeToPass;
	// 룰렛 회전 횟수(n바퀴)
	int _numberOfRouletteSpins;
	

	// 대기 시작 시간
	int64 _startTime;
	// 대기 끝인가
	bool _isWaitEnd = true;
	// 현재 애니 플레이 할 슬롯 id
	int _currentPlayAniSlotId = 0;
	// 현재 바퀴수(n바퀴째)
	int _currentRoundCount =0;

	// 탭(1성계, 2성계) 선택 토글 그룹
	FGsToggleGroup _starLegacyTabToggleGroup;

	// 가상 함수
protected:	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual bool SyncContentState() override;
	virtual bool OnBack() override;

	// 메시지 
public:
	// 별의 유산 등급 오픈 정보 변경
	void OnChangedStarLegacyMagnitudeOpenInfo(const IGsMessageParam* inParam);
	// 별의 유산 해제 정보 갱신
	void OnChangedStarLegacySlotReleaseInfo(const IGsMessageParam* inParam);
	// 별의 유산 선택 정보 갱신
	void OnChangedStarLegacySlotSelectInfo(const IGsMessageParam* inParam);
	// 별의 유산 룰렛 연출 시작
	void OnStartStarLegacyRoulette(const IGsMessageParam* inParam);
	// 별의 유산 슬롯 정보 재배치
	void OnRerollStarLegacy(const IGsMessageParam* inParam);
	// 별의 유산 카테고리 탭 변경
	void OnChangedStarLegacyCategoryTab(const IGsMessageParam* inParam);

	// 로직 함수
public:
	// 메시지 바인딩 초기화
	void InitializeMessage();
	// 메시지 바인딩 해제
	void FinalizeMessage();
	// 타이머 시작(한칸 진행)
	void StartWaitTimmer();
	// 슬롯 애니메이션 플레이
	void PlaySlotAnimation();
	// 룰렛 연출 데이터 초기화
	void InitializeRouletteData();
	// 다음 슬롯 처리
	void ProcNextSlot();
	// 룰렛 종료 호출
	void FinishRoulette();

	// bp 함수
public:
	// 소환 연출 시작
	UFUNCTION()
	void OnClickSummonStart();
	// 배치 변경
	UFUNCTION()
	void OnClickReroll();
	// 탭 클릭(1성계, 2성계)
	UFUNCTION()
	void OnClickTab(int32 In_index);

	// get, set
public:
	// 슬롯 구하기
	UGsUIStarLegacySlot* FindStarLegacySlot(int In_slotId);
	
	// 정보 세팅(탭 갱신)
	void SetStarLegacyInfo();
	// 남은 횟수 정보 세팅
	void SetRemainCount();

	// 룰렛 관련 패널 세팅
	void SetRoulettePanel(bool In_isRouletteStart);
	// 룰렛 연출 visible 세팅
	void SetAllRouletteSlotVislble(bool In_isVisibleOn);
	// 남은 bm 아이템 갯수 세팅
	void SetBMItemCount();
	// 소환 재화 세팅
	void SetSummonCurrency(const FGsSchemaStarLegacyCategory* In_categoryTbl);
	// 변경 재화 세팅
	void SetRerollCurrency(const FGsSchemaStarLegacyCategory* In_categoryTbl);
	// 현재 슬롯 정보 세팅
	void SetCurrentSlotInfo();
};
