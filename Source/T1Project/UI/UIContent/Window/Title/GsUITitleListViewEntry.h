#pragma once

/**
* file		GsUITitleListViewEntry.h
* @brief	Title List View Entry
* @author	PKT
* @date		2022/01/20
**/

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GsUITitleListViewEntry.generated.h"


/************************************************************************/
/* View List Entry Data                                                 */
/************************************************************************/
class FGsTitleData;

DECLARE_DELEGATE_TwoParams(FOnClickedTitleEquipChanged, int32, bool);

UCLASS()
class T1PROJECT_API UGsTitleListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	const FGsTitleData* _titleData = nullptr;

	FOnClickedTitleEquipChanged OnClickedTitleEquipChanged;
};




/************************************************************************/
/* View List UI Entry                                                   */
/************************************************************************/

class UImage;
class UGsButton;
class UTextBlock;
class UVerticalBox;
class URichTextBlock;
class UWidgetSwitcher;
class UGsUIRedDotDefault;
class UGsDynamicPanelSlotHelper;

UCLASS()
class UGsUITitleListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	/************************************************************************/
	/* BP Bind                                                              */
	/************************************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _textcondition;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _swithcerStatDisplay;

	// 2022/01/20 PKT - Stat Info
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfStat;

	// 2021/08/03 PKT - Stat List
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBoxStatList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textNotifyEmptyStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _swithcerChangedStateBtn;

	// 2022/01/20 PKT - btn
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEquip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUnEquip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageEquipEffectMarker;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDotNormal;
	
	
	const FGsTitleData* _titleData = nullptr;

	FOnClickedTitleEquipChanged OnClickedTitleEquipChanged;

public:
	virtual void BeginDestroy() override;	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;	
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

protected:
	UFUNCTION()
	void OnRefreshStatList(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickedBtnEquip();

	UFUNCTION()
	void OnClickedBtnUnEquip();

protected:
	void Refresh();

public:
	void Invalidate();

	const FGsTitleData* GetData() const;
};
