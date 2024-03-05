// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupGuildAuctionDirect.generated.h"


class UGsButton;
/**
 * 기사단 직접 분배 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildAuctionDirect : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 입찰 가능 대상 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelUserList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	// 아이템 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockStart;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBoxItem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	class UGsDynamicIconSlotHelper* _scrollBoxItemHelper;

	// 확인 및 취소 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	FGsToggleGroup _toggleGroupUserList;

	TArray<ItemDBId> _itemDBIdList;

	TArray<TPair<UserDBId, FString>> _userInfoList;
	UserDBId _selectedUserDBId = INVALID_USER_DB_ID;

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

	void OnClickUserName(int32 InIndex, bool bInIsSelected);

	void OnLongPressIcon(class UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnRefreshEntryUserList(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon);

	void InvalidateButton();

public:
	void SetData(const TArray<ItemDBId>& InDivisionItemDBIdList);

	void CheckItemValid();

};
