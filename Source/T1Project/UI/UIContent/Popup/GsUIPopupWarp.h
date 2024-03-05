#pragma once

#include "Currency/GsCostPackage.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UILib/Base/GsUIPopup.h"

#include "GsUIPopupWarp.generated.h"

/*
	popup 워프(위치 즐겨찾기, 지역맵 포탈)
*/

class UGsButton;
class UGsUICurrencySlot;

class UWidgetSwitcher;
class FGsCostPackage;

UCLASS()
class T1PROJECT_API UGsUIPopupWarp :public UGsUIPopup 
{
	GENERATED_BODY()
	
protected:
	// 비용 타입 스위칭(1: 재화, 2: 아이템)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCostType;

	// 1. 재화
	// 사용 비용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencySlotUse;
	// 현재 비용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencySlotNow;

	// 2. 아이템
	// 사용 아이템 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemCountUse;
	// 현재 아이템 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemCountNow;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorItemCountUse;

	// 텍스트: 이동할 타겟
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMoveTarget;
	// ok 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	// cancel 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	TFunction<void()> _callbackOkClick;

	// 이벤트 종료 호출 할지
	bool _isCallEventProcessFinish = true;

	TSharedRef<FGsCostPackage> _costPackage = MakeShared<FGsCostPackage>();

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

	// 이벤트
protected:
	// ok 클릭
	UFUNCTION()
	void OnClickOk();
	// cancel 클릭
	UFUNCTION()
	void OnClickCancel();

	// set
public:
	// 데이터 세팅
	void SetData(const FString& In_text,
		TSharedRef<FGsCostPackage> InCostPackage,
		TFunction<void()>In_callback = nullptr);
};
