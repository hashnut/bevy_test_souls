// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Currency/GsCostPackage.h"
#include "GsUIPopupCommonPay.generated.h"

class UGsUICurrencySlot;
class URichTextBlock;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCommonPay : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	class URichTextBlock* _contentRichText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	class URichTextBlock* _underContentRichText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencySlot* _currencyCommonPay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencySlot* _currencyLocalPay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOk;

	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

	// destruct 호출 시점에 
	// 이벤트 종료 관련 호출처리(EGsEventProgressType::UI_POPUP_YES_NO_CHNNEL_MOVE)
	// T, F: _isCallEventProcessFinish
	TFunction<void(bool)> _destructCallback;

	// 이벤트 종료 호출 할지
	bool _isCallEventProcessFinish = true;

	TSharedRef<FGsCostPackage> _costPackage = MakeShared<FGsCostPackage>();

public :
	// 기사단 창고 Tray위에 띄우기 위해 임시 처리
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_SYSTEM_POPUP; }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();

public:
	// CostPackage 소모할 경우
	void SetData(const FText& InContent, TSharedRef<FGsCostPackage> InCostPackage,
		TFunction<void(bool)> InCallback = nullptr,
		TFunction<void(bool)> InDestructCallback = nullptr);

	void SetUnderText(const FText& InText);
};
