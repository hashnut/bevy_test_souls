// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupGuildAuctionAppointment.generated.h"


class UPanelWidget;
class UUserWidget;
class UGsDynamicPanelSlotHelper;
class UTextBlock;
class UGsUIIconBase;
class UGsUIIconItemInventory;
class UGsUINumpadDefault;
class UGsButton;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItem;
struct FGsGuildDivisionItem;
/**
 * 기사단 지정 경매 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildAuctionAppointment : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 입찰 가능 대상 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelUserList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	// 아이템 수량에 따른 분기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherCount;

	// 아이템 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockItemName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsUIIconItemInventory* _uiIcon;

	// 입찰자 수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBidderCount;

	// 경매 시작 금액
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;

	// 숫자 패드
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUINumpadDefault* _uiNumpad;

	// 확인 및 취소 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	// 아이템 영역 : 2개 이상일 때 노출
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStart;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBoxItem;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelectorMulti;

	UPROPERTY()
	class UGsDynamicIconSlotHelper* _scrollBoxItemHelper;


	/**
	 * 지정 경매 : GuildAuctionUIText - Appointment_PopUp_Title
	 * 입찰자 : GuildAuctionUIText - Appointment_PopUp_Bidder
	 * 경매 시작 금액 : GuildAuctionUIText - Appointment_PopUp_BeginPrice
	 * 안내 문구 : GuildAuctionUIText - Appointment_PopUp_Description
	 */

protected:
	TArray<ItemDBId> _itemDBIdList;
	TArray<TPair<UserDBId, FString>> _userInfoList;
	TArray<UserDBId> _selectedUserList;
	Currency _biddingStartPrice = 0;
	Currency _minPrice = 0;
	GuildAuctionResultType _resultType = GuildAuctionResultType::MAX;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickOk();

	UFUNCTION()
	void OnClickCancel();

	void OnClickedUserName(UserDBId InUserDBId, bool InIsOn);

	void OnLongPressIcon(UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnRefreshEntryUserList(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

	void InvalidateBidderCount();
	void InvalidateButton();

	void OnChangeNumpadNumber(int32 InNumber);

	void SetUIIcon(UGsUIIconItemInventory* InIcon, ItemDBId InItemDBId);

public:
	void SetData(const TArray<ItemDBId>& InDivisionItemDBIdList, GuildAuctionResultType InResultType);

	void CheckItemValid();

};
