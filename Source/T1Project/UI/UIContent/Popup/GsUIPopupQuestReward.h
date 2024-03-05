// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/GsUIPopupQuestBase.h"
#include "GsUIPopupQuestReward.generated.h"

/**
 * 퀘스트 완료 팝업(랜덤 연출이 없는 모든 항목)
 */
UCLASS()
class T1PROJECT_API UGsUIPopupQuestReward : public UGsUIPopupQuestBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

protected:
	virtual void CloseInternal() override;

	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	virtual void SetData(const IGsMessageParam* InParam) override;

protected:
	bool SendRequestReward();
};
