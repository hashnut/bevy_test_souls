// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "GsUIChatGroupMemberEntry.generated.h"

class UGsSwitcherButton;
class UGsButton;
class UWidgetSwitcher;
class UGsUIChatGroupGuildData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIChatGroupMemberEntry : public UUserObjectScrollEntry
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnActionMember, GuildDBId);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FSlateBrush _emblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _guildName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _guildMasterName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _iconAdmin;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _authSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSelect; //select, unselect
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectAction;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEntrust;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnKickOut;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancelInvite;

	UObject* _data{ nullptr };

public:
	FOnActionMember OnEntrustMaster;
	FOnActionMember OnKickOut;
	FOnActionMember OnCancelInvitation;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickEntrust();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickKickOut();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickInvitationCancel();

protected:
	void RefreshMemberUI(UGsUIChatGroupGuildData* data, bool isControlable = false);
	void RefreshInvitationUI(UGsUIChatGroupGuildData* data, bool isControlable = false);
public:
	void RefreshUI();
};
