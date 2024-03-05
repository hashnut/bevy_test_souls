// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "DataSchema/CommunityGirl/GsSchemaCommunityDialog.h"
#include "GsUIWindowCommunity.generated.h"

class UCanvasPanel;
class UTextBlock;
class UGsButton;
class UGsUIAttendancePage;
class UWidgetSwitcher;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UPanelWidget;
class IGsCommunityGirlData;
//class IGsUICommunityGirlPage;
class FGsAttendanceData;
struct FGsSchemaCommunityEvent;
class UImage;
class UGsUICommunityEventMissionPage;
class URichTextBlock;

/**
 *  메뉴에서 커뮤니티소녀 버튼을 클릭 했을 때 열리는 풀창(window)	
 */
UCLASS()
class T1PROJECT_API UGsUIWindowCommunity : public UGsUIWindow
{
	GENERATED_BODY()		

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _talkBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _talkText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMainNotice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCommunity;	

	// 출석체크
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIAttendancePage* _attendancePage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICommunityEventMissionPage* _communityEventMissionPage;

	//  tab 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _tabScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _tabScrollPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgBanner;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textEventId;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _eventIdCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextEndTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _communityEventType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _overTimeCanvasPanel;

	//  tab 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _tabScrollBoxHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _tabScrollBoxEntryClass;

	// 선택 on/off 헬퍼
	FGsToggleGroup _tabToggleGroup;

	//  tab 현재 선택 메뉴
	UPROPERTY()
	int32 _tapIndex = -1;
	CommunityEventId _eventId = INVALID_COMMUNITY_EVENT_ID;

	float _dlgEndTime{ 0.f };
	TArray<CommunityEventId> _communityEventIdList;

	FDateTime _tmCondition;
	FText _textDay;
	FText _textHour;	
	FText _textMin;
	FText _textSec;
	FText _textCondition;
	FText _textOverTime;
	FText _textConditionComplated;
	float _updateSec = 1.0f;
	float _updateRemainSec = 0.0f; 

public:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickPromotionNotice();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCommunity();

protected:
	// tab 스크롤 업데이트
	UFUNCTION()
	void OnCreateTabButtonEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshTabButtonEntry(int32 inIndex, class UWidget* InEntry);

	// tab 클릭
	UFUNCTION()
	void OnClickTab(int32 InIndex);
	
	void OnClickTab(CommunityEventId inEventId, bool inIsOn);

public:
	virtual void BeginDestroy() override;
	virtual bool SyncContentState() override;
	// 자동이동 워프전 확인용
	// 커뮤니티 window는 막음
	virtual bool GetPreventAutoClose() const override { return true; }

protected:
	virtual void InvalidateAllInternal();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	
	virtual void CloseInternal() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	void UpdateCommunityEvent();

protected:
	virtual void OnClickBack() override;
	virtual void OnClickCloseAllStack() override;

protected:
	//void InitTabData();
	void SetBanner(const FGsSchemaCommunityEvent* inCommunityEvent);
	void UpdateTab();
	void UpdateConditionTime();
	void SetEventOverTime(bool inIsShow);

public:
	void ChangeDialog(const FGsSchemaCommunityDialog* dlg);
	void UpdateExpire();
	void UpdatePage(CommunityEventId inEventId, int inIndex);
};
