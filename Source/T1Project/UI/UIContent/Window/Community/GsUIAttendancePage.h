// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIAttendancePage.generated.h"

class UWidgetSwitcher;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UGsWrapBoxIconSelector;
class UGsUIIconBase;
class UGsUICurrencyButton;
class UGsButton;
class UTextBlock;
class URichTextBlock;
class UImage;
class UGsUIAttendanceItem;
class UCanvasPanel;
struct FGsSchemaCommunityEvent;
struct FGsSchemaCommunityEventAttendance;
class UScrollBox;
/**
 *  출석체크 판
 */
UCLASS()
class T1PROJECT_API UGsUIAttendancePage : public UUserWidget //, public IGsUICommunityGirlPage
{
	GENERATED_BODY()

protected:
	// == Attendance tab ======================================================
	
	// tab 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _tabScrollBoxEntryClass;

	//  tab 스크롤헬퍼
	/*UPROPERTY()
	class UGsDynamicPanelSlotHelper* _tabScrollBoxHelper;

	//  tab 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _tabScrollBox;

	// 선택 on/off 헬퍼
	FGsToggleGroup _tabToggleGroup;
	*/
	//  tab 현재 선택 메뉴
	UPROPERTY()
	int32 _tapIndex{ 0 };
	
	// == Attendance tab ======================================================

	// == Attendance List =====================================================
	// list 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _listWrapEntryClass;

	// list 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _listScrollBoxHelper;

	// list 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWrapBox* _listWrapBox;

	// list 현재 선택 Item
	UPROPERTY()
	int32 _itemIndex;

	// 버튼 스위치 ( 구매, 출첵 )
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class  UWidgetSwitcher* _btnSwitcher;

	// 보상 받기 (레드닷)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDot;

	UPROPERTY()
	bool _isRewardRevRedDot = false;

	// 구매 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencyButton* _btnPurchase;

	// 보상 받기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnRewardRev;

	// 보상 받기 버튼 Text
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBtnRewardRev;

	// 초기화 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _textResetData;

	// 유효 기간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textExpirationDate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textDayNum;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _deemedCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _noPageCanvasPanel;

	UPROPERTY()
	int _slotItem = 0;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIAttendanceItem* _dailyBonusItem;

	UPROPERTY()
	class UGsUIAttendanceItem* _selectItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textEndTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _attendanceIdCanvasPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAttendanceId;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	

protected:
	const FGsSchemaCommunityEvent* _schemaCommunityEvent;
	const FGsSchemaCommunityEventAttendance* _schemaAttendance;
	int _attendanceId = 0;
	bool _expire = false;
	bool _isInvalidateAll = false;

public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;

protected:
	// list 스크롤 업데이트
	UFUNCTION()
	void OnRefreshEntryListScrollBox(int32 inIndex, class UWidget* InEntry);

	// 구입
	UFUNCTION()
	void OnClickPurchase();

	// 보상
	UFUNCTION()
	void OnClickReward();

	// Page 업데이트
	UFUNCTION()
	void UpdatePage(int32 inIndex);

protected:
	bool GetResetTimeText(IN ResetTimeTicket inResetTimeTicket, OUT FText& outText);


public:
	void OnClickIconSlot(int inIndex, class UWidget* inItem);
	void OnClickDailyBonusSlot(int inIndex, class UWidget* inItem);

	void InvalidateAll();
	void OnSelectTab(int index);
	void OnSelectTab(int index, const FGsSchemaCommunityEvent* inData);

	void SetNoPage(bool inIsShow);

	void SetExpire();

public:
	void SetRewardScrollOffSet(float inValue);
	void UpdateRewardScrollOffset(int32 inIndex, UGsUIAttendanceItem* inItem);
};
