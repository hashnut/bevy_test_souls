#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Common/Menu/GsUIDropDownMenuCommon.h"
#include "GsUIPopupFairyCollection.generated.h"

/**
 *
 */
class UListView;
class UTextBlock;
class UGsButton;
class UPanelWidget;
class UWidgetSwitcher;
class UGsUISummonPortraitBase;
struct FGsFairyData;

UCLASS()
class T1PROJECT_API UGsUIPopupFairyCollection : public UGsUIPopup
{
    GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewCollections;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _fairyName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _listSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISummonPortraitBase* _portrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelFindFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFindFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _selectPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIDropDownMenuCommon* _levelSelector;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClose();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickFindFairy();

	UFUNCTION()
	bool OnInputSelected(int32 index, bool forced = false);

	UFUNCTION()
	void OnCreateMenuEntry(int32 index, struct FMenuData& menuData);

protected:
	TWeakPtr<FGsFairyData> _data;

public:
	struct PopupInitData
	{
		FairyId _id;
		FairyLevelId _level = (uint32)(~((uint32)0));

		bool _useFindItem{ true };
	};

protected:
	virtual void NativeOnInitialized() override;

public:
	void InitializeData(PopupInitData* param = nullptr);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
public:
	// 뽑기 상태에서 UI_HIDE_DEFAULT 로 안보여 별도로 만듬
	// https://jira.com2us.com/jira/browse/C2URWQ-8110
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_FAIRY_COLLECTION_POPUP; }
};