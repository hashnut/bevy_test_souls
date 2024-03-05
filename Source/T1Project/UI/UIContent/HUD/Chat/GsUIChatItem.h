#pragma once
#include "CoreMinimal.h"
#include "UserWidget.h"
#include "../GsUIHUDChat.h"
#include "GsUIChatItem.generated.h"

class URichTextBlock;
class UPanelWidget;
class UWidgetSwitcher;

struct FGsChatItem;

UCLASS()
class UGsUIChatItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _chatText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _invasionInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectInvasionMark;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _badgeInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _guildMark;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FSlateBrush _badge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FSlateBrush _emblem;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetChatData(TWeakPtr<FGsChatItem> data);
	void AddChatText(FString chatText);
	void ClearChatText();

};

