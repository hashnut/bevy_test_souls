// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIWindowMail.generated.h"

class UGsButton;
class UUserWidget;
class UScrollBox;
class UGsSwitcherButton;
class UTextBlock;
class UGsDynamicPanelSlotHelper;
class FGsMailInfo;
class UCanvasPanel;
/**
 *  메뉴에서 메일 버튼을 클릭 했을 때 열리는 풀창(window)	
 */
UCLASS()
class T1PROJECT_API UGsUIWindowMail : public UGsUIWindow
{
	GENERATED_BODY()
private:
	//static const int8 MAX_MAIL_COUNT = 50;
	static const int8 MAX_PAGE_SLOT_COUNT = 10;
	//static const int8 MAX_REWARD_ITEM_COUNT = 5;

protected:
	// 슬롯 class
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;

	// 우편 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMailCount;

	// 페이지 숫자
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textPageNum;

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;

	// 모두 받기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnReceiveAll;

	// 모두 삭제 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnDeleteAll;

	// 페이지 앞 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPagePrev;

	// 페이지 뒤 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPageNext;

	// 개인 탭 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switchBtnUser;

	// 길드 탭 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switchBtnGuild;

	// 계정 탭 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switchBtnAccount;

	// 개인 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _userTabRedDot;

	// 길드 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _guildTabRedDot;

	// 계정 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _accountTabRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _canvasPanelNoList;

	// 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

protected:
	// 토글 그룹
	FGsToggleGroup _toggleGroupTab;
	// 선택 된 탭 index
	int32 _selectedTabIndex = 0;
	// 선택 Page index
	int32 _pageIndex = 0;
	// 우편 목록
	TArray<TSharedPtr<FGsMailInfo>> _mailList;
//	TArray<MsgGameObjectHandle> _msgGameObjectHandleList;

	float _scrollOffset = 0;

public:
	virtual void BeginDestroy() override;	

protected:
	virtual void InvalidateAllInternal();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	
	virtual void CloseInternal() override;
	virtual bool SyncContentState() override;

public:
	UFUNCTION()
	void InIt();

protected:	
	// 슬롯 갱신
	UFUNCTION()
	void OnRefreshEntry(int32 inIndex, UWidget* InEntry);

	// 탭 클릭 됐을 때
	UFUNCTION()
	void OnClickTab(int32 inIndex);

	// 모둩 받기 눌렀을 때
	UFUNCTION()
	void OnClickReceiveAll();

	// 모두 삭제 눌렀을 때
	UFUNCTION()
	void OnClickDeleteAll();

	// 페이지 앞
	UFUNCTION()
	void OnClickPagePrev();

	// 페이지 뒤
	UFUNCTION()
	void OnClicPageNext();

private:
	// 리스트 갱신
	void InvalidateList(int inIndex);
	// 우편 갯수
	void UpdateTextCount();
	// 페이지 숫자
	void UpdatePageNum();
	// 레드닷 갱신
	void UpdateRedDot();
};
