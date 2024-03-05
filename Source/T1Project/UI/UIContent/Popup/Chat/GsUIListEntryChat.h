#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "GsUIListEntryChat.generated.h"

class URichTextBlock;
class UGsUIListDataChat;
class UGsButton;
class UWidgetSwitcher;
class UPanelWidget;

UCLASS()
class T1PROJECT_API UGsUIListEntryChat : public UUserObjectScrollEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNickName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _nickName;

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

	const int CHAT_UPDATE_TIME{ 60 };

	int _passTime{ 0 };

	UObject* _data{ nullptr };

public:
	TFunction<void(UGsUIListDataChat*)> _cbSelection = nullptr;
	bool IsValideCallback() { return _cbSelection != nullptr; }
	void SetSelectionCallback(TFunction<void(UGsUIListDataChat*)> cbSelection) { _cbSelection = cbSelection; }

	void SetData(UObject* ListItemObject);

	UObject* GetListItem() { return _data; };

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNickName();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};

