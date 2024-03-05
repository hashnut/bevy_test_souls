// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Chat/GsChatItem.h"
#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupChatOptionNBlock.generated.h"

/**
 * 
 */
class UScrollBox;
class UTextBlock;
class UPanelWidget;
class UEditableTextBox;
class UWidgetSwitcher;
class UGsButton;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;
class UGsUIChatOptionButton;

UCLASS()
class T1PROJECT_API UGsUIPopupChatOptionNBlock : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _touchClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _tabBtnChatSetting;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _tabBtnBlockList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _tabSwitcher;

	FGsToggleGroup _toggleGroup;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkSystem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkServer;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkAllience;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkKnights;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkParty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkInvade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkWhisper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _groupChatOptions;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _optionPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UGsUIChatOptionButton> _entryGroupButton;

	//UPROPERTY()
	//UGsDynamicPanelSlotHelper* _groupChatOptionHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkTalkBubble;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkEmoticon;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _userList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editEnterText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	TArray<FGsChatBlockUser> _blockList;
	TPair<MessageContentChat, FDelegateHandle> _msgHandler;

	int8 _selectedIndex = -1;

private:
	TMap<EGsOptionChat, UGsSwitcherButton*> _checkViewChat;
	TArray<UGsUIChatOptionButton*> _checkViewGroupChat;

	bool isInitialzed{ false };

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnTouchedExtraRange();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClose();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelectedTab(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickBlock();

	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntry(int32 index, UWidget* InEntry);

public:
	struct PopupInitData
	{

	};
	
	virtual void InitializeData(PopupInitData* initParam = nullptr);

	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void CloseInternal() override;

	virtual void OnExit();

	void OnSelectedAllCheck(bool selection);
	void OnSelectedTabCheck(EGsOptionChat type, bool selection);
	void OnSelectedGroupCheck(uint64 roomId, bool selection);

	void OnEmojiPopCheck(bool selection);
	void OnTalkBubblePopCheck(bool selection);

	void OnChatBlockListUpdate(const IGsMessageParam* param);
	void OnReqChatUserUnblock(UserDBId uid);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
