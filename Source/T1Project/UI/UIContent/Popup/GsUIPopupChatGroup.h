// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupChatGroup.generated.h"

class UGsScrollBox;
class UGsSwitcherButton;
class UGsButton;
class UGsUIChatGroupPageRoomInfo;
class UGsUIChatGroupPageInvitation;
class UGsUIChatRoomListEntry;

class UWidgetSwitcher;
class UPanelWidget;

class FGsGuildGroupChatRoom;
/**
 * Guild Chat Group Management and Information Popup
 */
UCLASS()
class T1PROJECT_API UGsUIPopupChatGroup : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textRoomCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsScrollBox* _chatRoomList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _detailSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatGroupPageRoomInfo* _roomInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatGroupPageInvitation* _invitaionList;

private:
	TArray<TWeakPtr<FGsGuildGroupChatRoom>> _roomList;
	TArray<TPair<MessageContentChat, FDelegateHandle>> _chatMessageHandlers;

	TWeakPtr<FGsGuildGroupChatRoom> _selectData;

protected:
	UFUNCTION(BlueprintCallable)
	void OnCloseExtra();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void OnUpdateChatGroupInfo(const IGsMessageParam* param);
	void OnUpdateChatGroupMemberInfo(const IGsMessageParam* param);
	void OnUpdateChatGroupGuestInfo(const IGsMessageParam* param);
	void OnUpdateChatGroupInvitaion(const IGsMessageParam* param);
	void OnRemoveChatGroupInvitaion(const IGsMessageParam* param);
	void OnUpdateChatGroupEntrusted(const IGsMessageParam* param);
	void OnUpdateChatGroupNameChanged(const IGsMessageParam* param);
	void OnUpdateChatGroupAuthGrade(const IGsMessageParam* param);

protected:
	void OnCreateChatGroup();
	void OnPopupInvitationList();
	void OnReqRoomInfo(TWeakPtr<FGsGuildGroupChatRoom> data);
	void OnEditRoomName(TWeakPtr<FGsGuildGroupChatRoom> data);

	void SetEntryFunction(UGsUIChatRoomListEntry* roomEntry);
	void ResetEntryFunction(UGsUIChatRoomListEntry* roomEntry);
};
