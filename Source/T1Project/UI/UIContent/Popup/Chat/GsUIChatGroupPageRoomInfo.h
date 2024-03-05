// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIChatGroupPageRoomInfo.generated.h"

class UGsScrollBox;
class UGsSwitcherButton;
class UGsButton;
class UGsUIDropDownMenuChat;
class UWidgetSwitcher;
class UPanelWidget;
class UGsUIChatGroupMemberEntry;

class FGsGuildGroupChatRoom;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIChatGroupPageRoomInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	FGsToggleGroup _toggleGroupTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _tabBtnGroupChat;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _tabBtnInvite;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _tabPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMemberCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textInvitingCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectNoData;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsScrollBox* _memberGuildList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _bottomInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInvite;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLeave;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIDropDownMenuChat* _chatGroupTargetGrade;

	TWeakPtr<FGsGuildGroupChatRoom> _curData;

	GuildDBId _myGuildDBId{ INVALID_GUILD_DB_ID };

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTabChanged(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	bool OnGradeSelected(int32 index, bool forced = false);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickInvite();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickLeave();

	void OnEntrustAuth(GuildDBId guildId);
	void OnKickOutGuild(GuildDBId guildId);
	void OnCancelInvitation(GuildDBId guildId);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void OnSelectMemberTab();
	void OnSelectInvitationWaitingTab();

	void SetEntryFunction(UGsUIChatGroupMemberEntry* entry);

public:
	void InitializeData(TWeakPtr<FGsGuildGroupChatRoom> weakData);
	void RefreshUI();
};
