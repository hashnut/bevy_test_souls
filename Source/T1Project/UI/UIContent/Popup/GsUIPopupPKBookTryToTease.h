// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUIPopupPKBookTryToActionBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupPKBookTryToTease.generated.h"

class UGsButton;
class UGsCheckBox;
class URichTextBlock;
class UGsUICurrencySlot;
class UEditableTextBox;

/**
 * 상태이상 상세 정보창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupPKBookTryToTease : public UGsUIPopupPKBookTryToActionBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextMessage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsCheckBox* _checkBoxUseCustomMsg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _textMockeryMessage;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;

	virtual void TryToAction(PKRecordId InRecordId) override;

	UFUNCTION()
	void OnTextChangedMsg(const FText& InText);

public:
	void SetData(PKRecordId InRecordId, const struct FGsPKRecordingContributorData* InData, bool InUseMockeryMessage, const FText& InMockeryMessage);
};
