// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "GsUIWindowQuest.generated.h"

class UGsUIWindowQuestTabBase;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsUIRedDotBase;

class UGsUIWindowQuestMain;
class UGsUIWindowQuestSub;
class UGsUIWindowQuestRepeat;
class UGsUIWindowQuestGuide;


/**
 *  메뉴에서 퀘스트 버튼을 클릭 했을 때 열리는 풀창(window)	
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuest : public UGsUIWindow
{
	GENERATED_BODY()
protected:
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _tabSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switchBtnMain;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switchBtnSub;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switchBtnRepeat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switchBtnGuide;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIRedDotBase* _tabSubRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIRedDotBase* _tabRepeatRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIRedDotBase* _tabGuideRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIWindowQuestMain* _questMainArea;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIWindowQuestSub* _questSubArea;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIWindowQuestRepeat* _questRepeatArea;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIWindowQuestGuide* _questGuideArea;

	FGsToggleGroup _toggleGroupTab;
	int32 _selectedTabIndex = 0;

	MsgGameObjHandleArray _msgGameObjectHandleList;
	TArray<MsgHandleUnlock> _msgUnlockHandleList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual void CloseInternal() override;
	virtual bool SyncContentState() override;

protected:
	UFUNCTION()
	void OnClickTab(int32 InIndex);
	UFUNCTION()
	void OnBlockedTab(int32 InIndex);
	UFUNCTION()
	void CheckRedDot();
	UFUNCTION()
	void CheckUnlock(bool InIsFirstCheck = false);


private:
	// 메세지 등록
	void RegisterMessages();
	// 메세지 해지
	void DeregisterMessages();
	// window창이 열려 있을 때 퀘스트 변화가 생기면 받는 메세지 (완료,포기..등등의 )
	void OnWindowQuestUpdate(const IGsMessageParam* In_data);	
	// window창이 열려 있을 때 퀘스트 오브젝티브 변화가 생기면 받는 메세지
	void OnWindowQuestObjectiveUpdate(const IGsMessageParam* In_data);
	// 켄텐츠의 잠금이 풀렸을 때 받는 메세지 (서브퀘스트)
	void OnContentsUnlock(const struct IGsMessageParam* InParam);

public:
	void OnEventClose();
};
