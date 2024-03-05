// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupUserProfileSetting.generated.h"


class UGsButton;
class UMultiLineEditableText;
/**
 * 유저 프로필 설정 팝업 UI
 */
UCLASS()
class T1PROJECT_API UGsUIPopupUserProfileSetting : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UMultiLineEditableText* _inputBoxIntroduction;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnTextChangedIntroduction(const FText& InText);
	
	// 2022/10/20 PKT - KeyBoard Mapping
protected:	
	virtual void OnInputCancel() override;
};
