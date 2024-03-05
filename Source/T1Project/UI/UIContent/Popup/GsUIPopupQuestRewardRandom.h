// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/GsUIPopupQuestBase.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageGameObject.h"
#include "GsUIPopupQuestRewardRandom.generated.h"

struct IGsMessageParam;

/**
 * 퀘스트 완료 팝업(랜덤 연출이 있는 항목)
 */
UCLASS()
class T1PROJECT_API UGsUIPopupQuestRewardRandom : public UGsUIPopupQuestBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _randomWaitTime = 4.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;

protected:
	MsgUIHandleArray _msgUIHandleList;
	MsgGameObjHandleArray _msgGameObjectHandleList;

	float _remainTime = 0.f;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void CloseInternal() override;

	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	virtual void SetData(const IGsMessageParam* InParam) override;

protected:
	void BindMessages();
	void UnbindMessages();

protected:
	bool SendRequestRepeatRewardBox();
	void SendRequestReward();

protected:
	void OnReceiveRepeatRewardBox(const IGsMessageParam* InParam);

	void OnReceiveRandomItemBagIndex(const IGsMessageParam* InParam);
	void OnRecvRewardResultFailed(const IGsMessageParam* InParam);
};
