#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "Summon/GsSummonDefine.h"

#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageUI.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIPopupSummonMileage.generated.h"
/*
* popup summon 마일리지
*/

class UGsButton;
class UGsDynamicPanelSlotHelper;

class UVerticalBox;
class UWidget;

struct IGsMessageParam;

UCLASS()
class  UGsUIPopupSummonMileage : public UGsUIPopup
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	// 확인 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

private:
	// 뽑기 타입(페어리, 코스튬)
	EGsSummonType _summonType;
	// 메시지 바인딩
	TPair<MessageUserInfo, FDelegateHandle> _handlerUserInfo;

	MsgUIHandleArray _uiMsgArray;
	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 이건 esc 키
	// https://jira.com2us.com/jira/browse/C2URWQ-8128
	virtual void OnInputCancel() override;

public:
	// 뽑기 상태에서 UI_HIDE_DEFAULT 로 안보여 별도로 만듬
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SUMMON_MILEAGE_POPUP; }
	
	// 로직함수
public:
	// 초기화
	void InitSummonMileage(EGsSummonType In_summonType);
	
	// 재화 갱신 메시지
	void OnUpdateCurrency(uint64 InType);
	// 소환 마일리지 데이터 갱신
	void OnChangedSummonMileageData(const  IGsMessageParam* inParam);
	

	// bp function
public:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
	// OK 클릭
	UFUNCTION()
	void OnClickOK();

	// set
public:
	// 현재 마일리지 데이터로 세팅
	void SetCurrentMileageData();

};