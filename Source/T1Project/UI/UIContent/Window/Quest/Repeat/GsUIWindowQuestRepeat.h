// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageGameObject.h"
#include "../Tab/GsUIWindowQuestTabBase.h"
#include "Quest/GsQuestData.h"
#include "GsUIWindowQuestRepeat.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
class UGsUICurrencyRefreshCountButton;
class UGsUIWindowQuestRepeatEntry;
struct FGsSchemaQuestRepeatStory;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestRepeat : public UGsUIWindowQuestTabBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherNonActive;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _btnSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGiveUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnTeleport;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAutoMove;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _questIdText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _storyIdText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencyRefreshCountButton* _btnRefresh;

	UPROPERTY()
	TArray<class UGsUIWindowQuestRepeatItem*> _treeRootSlotList;

	UPROPERTY()
	TArray<class UGsUIWindowQuestRepeatItem*> _treeItemPool;

private:
	MsgGameObjHandleArray _msgGameObjectHandleList;

protected:
	QuestKey _questKey = QuestKey::KeyNone();
	int _selectSlotIndex = 0;
	TArray<QuestKey> _repeatQuestBoardProgressList;
	bool _isSlotMoveded = false;

protected:
	// 포기
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickGiveUp();

	// 텔레포트
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickTeleport();

	// 자동이동
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAutoMove();

	// 보상받기
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickReward();

	// 새로고침
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickRefresh();

public:
	virtual void BeginDestroy() override;
	virtual void RefreshAll() override;
	virtual void RefreshQuestObjective() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void UpdateList() override;
	virtual void OnGiveUp() override;

protected:
	UGsUIWindowQuestRepeatItem* CreateTreeItem();
	void ReleaseTreeItem(UGsUIWindowQuestRepeatItem* inItem);
	void ReleaseAllTreeItem();
	void FreeTreeItem();

protected:
	void OnEntryWidgetGenerated(UUserWidget& userWidget);
	void OnEntryWidgetReleased(UUserWidget& userWidget);
	void OnClickedListViewItem(UObject* InListItem);
	void OnClickHudUIOnOffBtn(int32 inStoryId, int32 inIsOn);

public:
	void OnClickTab();
	void OnClickHudUIUpBtn(int32 inSlotIndex, int32 inIsUp);
	void OnClickHudUIDownBtn(int32 inSlotIndex, int32 inIsUp);

private:
	void OnClickedListEntry(UObject* InListItem);

protected:
	void BindMessages();
	void UnbindMessages();

protected:
	void OnReceiveRepeatRewardBox(const IGsMessageParam* InParam);
	void OnReceiveRepeatUpdate(const IGsMessageParam* InParam);
	void OnReceiveRepeatReAccept(const IGsMessageParam* InParam);

private:
	// 스토리 셋팅
	void SetData(StoryId inStoryId, QuestKey inQuestKey);
	// 진행 상태에 따른 버튼처리 (보상받기가 나올것인지, 자동이동 등이 나올것인지)
	void SetQuestSlotState(QuestState inQuestState);
	// 진행중인 퀘스트 카운트 / 진행가능 MAX 퀘스트 카운트
	void SetWorkingCount();
	// 보상 셋팅
	void SetReward(const FGsSchemaQuestRepeatStory* inSchemaQuestStory);
	// 새로고침 셋팅
	void SetRefreshData(QuestId inQuestId, QuestIndex inQuestIndex);
	// 반복퀘스트 스토리 타입
	RepeatStoryType GetRepeatStoryType(StoryId inStoryId);
	// 슬롯 정보
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> GetRepeatItem(StoryId inStoryId, QuestKey inQuestKey);

	QuestWindowUISlotType GetRepeatSlotType(StoryId inStoryId, QuestIndex inQuestIndex);
	const struct FGsSchemaQuestRepeatScrollRefreshInfoSet* GetRefrehTable(int32 inRepeatScrollId);

	// 데이터 검증
	bool IsVaildData(QuestId inQuestId, QuestIndex inQuestIndex);
};
