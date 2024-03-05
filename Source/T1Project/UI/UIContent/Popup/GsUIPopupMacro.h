#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
//#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Chat/GsChatItem.h"
#include "GsUIPopupMacro.generated.h"

/*
*
*/
class UGsSwitcherButton;
class UPanelWidget;
class UWidgetSwitcher;
class UGsUIChatMacroEntry;
//class UUniformGridPanel;

UCLASS()
class T1PROJECT_API UGsUIPopupMacro : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _tabBtnMacro;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switchWindow;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _tabWndMacro;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro6;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro7;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro8;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro9;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro10;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _touchClose;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnTouchedExtraRange();

public:
	struct PopupInitData
	{
		int selectedTab{ 0 };
		EGsChatViewType selectChatType{ EGsChatViewType::Max };
		TFunction<void(const FText&)> reqInput{ nullptr };
		TFunction<void()> cancel{ nullptr };
	};

protected:

	TFunction<void(const FText&)> _cbReqInput{ nullptr };
	TFunction<void()> _cbCancel{ nullptr };

	int8 _selectedIndex = -1;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClose();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void CloseInternal() override;

	void OnExit();

public:
	virtual void InitializeData(PopupInitData* initParam = nullptr);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};

