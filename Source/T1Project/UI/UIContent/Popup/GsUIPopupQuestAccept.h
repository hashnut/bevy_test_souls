// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/GsUIPopupQuestBase.h"
#include "GsUIPopupQuestAccept.generated.h"

struct IGsMessageParam;

/**
 * 퀘스트 수락 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupQuestAccept : public UGsUIPopupQuestBase
{
	GENERATED_BODY()

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

protected:
	virtual void CloseInternal() override;

	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	virtual void SetData(const IGsMessageParam* InParam);
};
