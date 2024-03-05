#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIPopupCostumeCollection.generated.h"

/**
 *
 */
class UListView;
class UTextBlock;
class UGsButton;
class UWidgetSwitcher;
class UGsUISummonPortraitBase;
class UPanelWidget;

struct FGsCostumeData;

UCLASS()
class T1PROJECT_API UGsUIPopupCostumeCollection : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewCollections;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISummonPortraitBase* _portrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _listSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelFindCostume;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFindCostume;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClose();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickFindCostume();

protected:
	TWeakPtr<FGsCostumeData> _data;
public:
	struct PopupInitData
	{
		CostumeId _id;
		CreatureGenderType _genderType;
		bool _useFindItem{ true };

		enum { InCostume = 0, InExchangeCostume = 1 } _type{ InCostume };
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
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_COSTUME_COLLECTION_POPUP; }
};