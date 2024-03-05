// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "GsUIChatRoomListEntry.generated.h"

class UGsButton;
class UGsSwitcherButton;
class UWidgetSwitcher;
class UPanelWidget;
class FGsGuildGroupChatRoom;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIChatRoomListEntry : public UUserObjectScrollEntry
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnSelectRoomCreate);
	DECLARE_DELEGATE_OneParam(FOnSelectRoomInfo, TWeakPtr<FGsGuildGroupChatRoom>);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switchGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _groupName;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _memberCount;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnRoomInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGroupEdit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _newRoomRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCreateRoom;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _invitaionPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRecvInvitation;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _invitationRedDot;

	UObject* _data{ nullptr };

public:
	FOnSelectRoomCreate OnRoomCreate;
	FOnSelectRoomCreate OnRecvInvitation;
	FOnSelectRoomInfo	OnSetRoomInfo;
	FOnSelectRoomInfo	OnEditRoomInfo;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCreateRoom();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickRecvInvitation();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickReqInfoRoom(bool set);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickEditGroup();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

protected:
	void RefreshUI();
	//void SetIsSelected(bool selected);
};
