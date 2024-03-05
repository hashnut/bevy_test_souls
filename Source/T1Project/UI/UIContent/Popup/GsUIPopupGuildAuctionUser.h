// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupGuildAuctionUser.generated.h"


class UTextBlock;
class UGsButton;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UUserWidget;
class UGsUIGuildAuctionUserEntry;
/**
 * 기사단 경매 참여자 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildAuctionUser : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 팝업 상단 이름 : 참여자 ({0})
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

	// 참여자 목록 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxUser;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	// 획득자 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildAuctionUserEntry* _uiMemberAcquirer;

	// 확인 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

protected:
	TArray<FString> _userNameList;

	ItemDBId _divisionItemDBId = INVALID_ITEM_DB_ID;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	virtual void OnInputCancel() override;
	virtual void OnInputOk() override;

protected:
	UFUNCTION()
	void OnClickOk();

	UFUNCTION()
	void OnRefreshEntryUser(int32 InIndex, class UWidget* InEntry);

public:
	void SetData(ItemDBId InDivisionItemDBId);
	void InvalidateList();

};
