// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/GsUIPopupQuestBase.h"
#include "GsUIPopupQuestAccept.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupQuestAcceptRepeat.generated.h"

class UTextBlock;
class UGsUICurrencyRefreshCountButton;
struct IGsMessageParam;

/**
 * 퀘스트 수락 팝업 - 새로고침 버튼 존재
 */
UCLASS()
class T1PROJECT_API UGsUIPopupQuestAcceptRepeat : public UGsUIPopupQuestAccept
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _iconImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyRefreshCountButton* _btnRefresh;

private:
	MsgGameObjHandleArray _msgGameObjectHandleList;
	
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void SetData(const IGsMessageParam* InParam) override;

protected:
	void BindMessages();
	void UnbindMessages();

protected:
	bool SendRequestRepeatRewardBox();

protected:
	void SetRefreshData(QuestId inQuestId, QuestIndex inQuestIndex);

protected:
	void OnReceiveRepeatRewardBox(const IGsMessageParam* InParam);

public:
	void OnClickRefresh();

protected:
	const struct FGsSchemaQuestRepeatScrollRefreshInfoSet* GetRefrehTable(int32 inRepeatScrollId);
};
 