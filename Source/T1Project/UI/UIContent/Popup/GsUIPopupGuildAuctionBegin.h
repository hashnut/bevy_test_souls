// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

#include "GsUIPopupGuildAuctionBegin.generated.h"


class UGsButton;
class UTextBlock;
class UScrollBox;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UUserWidget;
class UGsUIIconBase;
class UGsUIIconItem;
struct FGsGuildDivisionItem;
/**
 * 기사단 경매 시작 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildAuctionBegin : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStart;

	// 아이템 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxItem;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	class UGsDynamicIconSlotHelper* _scrollBoxHelper;

protected:
	TArray<ItemDBId> _itemDBIdList;

	GuildAuctionType _auctionType = GuildAuctionType::NORMAL;
	GuildAuctionResultType _resultType = GuildAuctionResultType::NORMAL;

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickOk();

	UFUNCTION()
	void OnClickCancel();

	UFUNCTION()
	void OnRefreshEntryItem(int32 InIndex, UGsUIIconBase* InIcon);
	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	void SetData(TArray<ItemDBId> InItemList, GuildAuctionType InAuctionType, GuildAuctionResultType InResultType);

	void CheckItemValid();

};
