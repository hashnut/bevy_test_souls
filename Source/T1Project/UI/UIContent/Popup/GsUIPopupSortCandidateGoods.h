#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "SortCandidateGoods/GsUICandidateGoodsListItem.h"

#include "GsUIPopupSortCandidateGoods.generated.h"

class FGsCostPackage;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UScrollBox;
class UTextBlock;
class UWidget;

UCLASS()
class UGsUIPopupSortCandidateGoods : public UGsUIPopup
{

	GENERATED_BODY()

public:

	struct Parameters
	{

		TWeakPtr<const FGsCostPackage> _costPackage;
		uint8 _costElementIndex;
		FSimpleDelegate _onClickedOk;

	};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UGsUICandidateGoodsListItem> _subClassOfList;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

private:
	Parameters _parameters;

	TArray<TSharedRef<UGsUICandidateGoodsListItem::Parameters>> _listParameters;

protected:
	void NativeOnInitialized();

	void NativeDestruct();

public:
	void SetParameters(Parameters InParameters);

private:
	void Invalidate();

	/** 현재 sort order 중 가장 큰 값. */
	int32 GetCurrentMaxSortOrder() const;

	/************************************************************************/
	/* events                                                               */
	/************************************************************************/

protected:
	UFUNCTION()
	void OnRefreshEntryListItem(int32 InIndex, UWidget* InEntry);

	void OnClickedListItemCraft(const int32 InIndex);

	void OnClickedListItemSort(const int32 InIndex);

	UFUNCTION()
	void OnClickedCancel();
	
	UFUNCTION()
	void OnClickedOk();

public:
	static UGsUIPopupSortCandidateGoods* OpenThis(Parameters InParameters);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};