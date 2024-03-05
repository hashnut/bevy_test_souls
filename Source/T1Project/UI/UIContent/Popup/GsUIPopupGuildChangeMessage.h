// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupGuildChangeMessage.generated.h"

class UGsButton;
class UTextBlock;

/**
 * 길드 공지사항/소개글 변경창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildChangeMessage : public UGsUIPopup
{
	GENERATED_BODY()

public:
	enum EGsGuildMsgType
	{
		INTRODUCTION,
		NOTICE,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UMultiLineEditableText* _inputBoxMsg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

protected:
	int32 _maxMsgCount = 0;
	EGsGuildMsgType _msgType = EGsGuildMsgType::INTRODUCTION;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetMessageType(EGsGuildMsgType InType);
	
protected:
	UFUNCTION()
	void OnClickOk();

	UFUNCTION()
	void OnClickCancel();

	UFUNCTION()
	void OnTextChangedMsg(const FText& InText);
};
