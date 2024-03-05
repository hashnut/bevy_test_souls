// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupReservationCreateNickName.generated.h"

class FGsCustomizeHandler;
class UTextBlock;
class UGsButton;
/**
 * UGsUIPopupReservationCreateNickName
 * 사전 닉네임 생성 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupReservationCreateNickName : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _nickName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCancel;

protected:
	TFunction<void(const FString&)> _okCallback;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetOkCallback(TFunction<void(const FString&)> InOkCallback = nullptr);
	void CallOkCallback();

public:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOK();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();
};
