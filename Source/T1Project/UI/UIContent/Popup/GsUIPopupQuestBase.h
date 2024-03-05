// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsUIPopupQuestBase.generated.h"

class UGsUIRewardInfoBase;
struct IGsMessageParam;
/**
 * 퀘스트 수락/완료 팝업의 부모 클래스. 중복기능이 많아 부모를 따로 둠.
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIPopupQuestBase : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _titleText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _descText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _typeText;

	// 보상. 보상유형에 따라서 주보상만 찍거나 선택보상도 찍거나 할 수 있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRewardInfoBase* _rewardInfo;

protected:
	QuestId _questId;
	QuestIndex _questIndex;
	QuestType _questType;
	StoryId _storyId;
	SpawnId _spawnId;
	RewardId _rewardMainId = INVALID_REWARD_ID;
	RewardId _rewardSubId = INVALID_REWARD_ID;
	QuestContentsType _questContentsType = QuestContentsType::NORMAL;

private:
	// 치트
	bool _bIsTest = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void CloseInternal() override;

	// 주의: OnClickOk, OnClickCancel이 아닌 OnInputOk, OnInputCancel을 오버라이딩하여 구현할 것
protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOk();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

public:
	//virtual void InputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent) override;
	//void InputMapKeyAxis(float Value) {};

public:
	virtual void SetData(const IGsMessageParam* InParam);

public:
	void SetRewardData(RewardId InRewardId, RewardId InSubRewardId);
	void SetQuestTypeText();

public:
	void SetIsTest(bool bIsTest);
	bool IsTest() const;
};
