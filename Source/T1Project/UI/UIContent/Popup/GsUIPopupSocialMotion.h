// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageUI.h"
#include "GsUIPopupSocialMotion.generated.h"

class UGsButton;
class UGsSwitcherButton;
class UPanelWidget;
class UWidgetSwitcher;
class UUniformGridPanel;
class UGsUIEmojiChatEntry;
class UGsUISocialMotionEntry;

struct FSocialEmotionMotion;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSocialMotion : public UGsUIPopup
{
	GENERATED_BODY()

public:
	const int32 EmojiColumnCount = 4;
	const int32 MotionColumnCount = 4;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _tabBtnEmoji;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _tabBtnMotion;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _tabWndEmoji;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _tabWndMotion;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUniformGridPanel* _emojiPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUniformGridPanel* _motionPanel;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryEmojiWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryMotionWidget;

protected:
	FGsToggleGroup _toggleGroup;
	TArray<UGsSwitcherButton*> _listItems;

	int32 _selectedIndex{ -1 };
	bool _isEnableMotion{ false };
	TArray<MsgUIHandle> _messageHandlers;

private:
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UGsUIEmojiChatEntry>> _emojiWidgets;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UGsUISocialMotionEntry>> _motionWidgets;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClose();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTabChanged(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelectedEmoji(int32 emojiId);

	void OnSelectedSocialMotion(TWeakPtr<FSocialEmotionMotion> data);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTouchedExtraRange();

protected:
	void DataInitialize();

	void OnMessageUpdateUI(const IGsMessageParam* param);
	void OnMessageEnableMotion(const IGsMessageParam* param);

protected:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	virtual void OnInputCancel() override;
};
