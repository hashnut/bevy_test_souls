// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "GsUIChatGroupInvitaionEntry.generated.h"

class UGsSwitcherButton;
class UGsButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIChatGroupInvitaionEntry : public UUserObjectScrollEntry
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnInvitation, GuildGroupChatRoomId, bool);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _newInvitationRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _chatRoomName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FSlateBrush _emblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _guildName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _guildMasterName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAccept;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReject;

	UObject* _data{ nullptr };

public:
	FOnInvitation OnResponseInvitation;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAccept();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickReject();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

public:
	void RefreshUI();
};
