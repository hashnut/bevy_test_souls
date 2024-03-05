#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIStarLegacySlot.generated.h"

/*
* 별의 유산 슬롯
*/

class UWidgetSwitcher;
class UPanelWidget;
class UWidgetAnimation;

class UGsHorizontalBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconBase;
class UGsButton;
class UGsUIIconItem;

class FGsStarLegacySlotArrangementInfo;

UCLASS()
class UGsUIStarLegacySlot : public UUserWidget
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	// 별의 유산 아이콘 셀렉터
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _targetStarLegacyRewardIconSelector;
	// 별의 유산 아이콘 생성하는거
	UPROPERTY()
	UGsDynamicIconSlotHelper* _targetStarLegacyRewardIconSlotHelper;

	// 등급 표시
	// 0: 닫혀있음
	// 1: 5등급
	// 2: 4등급
	// 3: 3등급
	// 4: 2등급
	// 5: 1등급
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherMagnitudeInfo;
	// 등급 오픈 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMagnitudeOpen;

	// 선택 표시 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSelect;
	// 룰렛 연출 표시 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRoulette;
	// 성공 표시 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuccess;
	// 실패 표시 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelFail;

	// 룰렛 연출 애니매이션
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _rouletteAnimation;

private:
	// 슬롯 정보
	FGsStarLegacySlotArrangementInfo* _slotInfo;

	// 등급 오픈 되었는지
	bool _isOpen = false;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// bp call function
public:
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateTargetIcon(UGsUIIconBase* InIcon);
	// 등급 오픈 클릭
	UFUNCTION()
	void OnClickMagnitude();
	// 아이콘 롱프레스 누를때
	void OnLongPressIconSlot(UGsUIIconItem& InIcon);
	// 아이콘 클릭 누를때(선택)
	void OnClickIconSlot(UGsUIIconItem& InIcon);

	// 로직함수
public:
	// 룰렛 애니 시작
	void StartRouletteAnimation();

	// 애니 초기화
	void InitAnimation();

	// set
public:
	// 슬롯 정보 인포로 세팅
	void SetSlotInfo(FGsStarLegacySlotArrangementInfo* In_info);	
	// 등급 상태 표시
	void SetMagnitudeInfo(bool In_isOpen);
	// 선택 표시 처리
	void SetSelectVisible(bool In_isSelect);
	// 성공, 실패 표시
	void SetSuccessOrFailVisible(bool In_isSuccess);
	// 룰렛 연출 표시 처리
	void SetRouletteSlotVisible(bool In_isVisibleOn);
};