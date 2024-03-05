// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Message/GsMessageContents.h"

#include "GsUIGuildAuctionChat.generated.h"


class UWidgetSwitcher;
class UGsScrollBox;
class UGsEditableTextBox;
class UGsButton;
struct IGsMessageParam;
/**
 * 기사단 경매 페이지 - 기사단 채팅
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAuctionChat : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	using MsgChatHandle = TPair<MessageContentChat, FDelegateHandle>;

protected:
	DECLARE_DELEGATE(FOnClickedMacro);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* _noHistorySelection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsScrollBox* _scroll_Chat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsEditableTextBox* _editEnterText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMacro;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnResetWhispererName;

public:
	FOnClickedMacro OnClickedMacro;

protected:
	TArray<MsgChatHandle> _msgChatHandleList;

	int32 _guildChatPos = 0;
	int32 _preChatTop = 0;

	bool _isUserScrolled = false;
	bool _toBottomScrolled = true;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void Show();

	UFUNCTION()
	void OnInputChat(const FText& InChatText);

protected:
	void InvalidateChatListItem();

	void OnChatListAdded(const IGsMessageParam* InParam);
	void OnChatListInserted(const IGsMessageParam* InParam);
	void OnChatListRefresh(const IGsMessageParam* InParam);
	void OnOperateMessageAdded(const IGsMessageParam* InParam);

	UFUNCTION()
	void OnUserScrolledChat(float InOffset);
	UFUNCTION()
	void OnClickMacro();
	UFUNCTION()
	void OnChangedText(const FText& InChangedText);
	UFUNCTION()
	void OnCommitedText(const FText& InInputText, ETextCommit::Type InCommitType);
};