#pragma once

#include "Runtime/Core/Public/Math/NumericLimits.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUICandidateGoodsListItem.generated.h"

class UGsButton;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsSwitcherButton;
class UGsUIIconBase;
class UTextBlock;

UCLASS()
class UGsUICandidateGoodsListItem : public UUserWidget, public IGsEntryInterface
{

	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FGsCandidateGoodsListItemDelegate, const int32 InIndex);

public:
	struct Parameters
	{

		static constexpr uint8 INVALID_SORT_ORDER = TNumericLimits<uint8>::Max();

		uint8 _index;
		ItemId _itemId;
		Level _itemLevel;
		uint64 _amount;

		uint8 _sortOrder;

		bool _canCraft;
		FGsCandidateGoodsListItemDelegate _onClickCraft;
		FGsCandidateGoodsListItemDelegate _onClickSort;

		bool IsValidSortOrder() const
		{
			return _sortOrder != INVALID_SORT_ORDER;
		}

	};

private:
	TWeakPtr<Parameters> _parameters;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCraft;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSort;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtOnSort;

protected:
	void NativeConstruct() final;

	void NativeOnInitialized() final;

	void NativeDestruct() final;

public:
	void SetParameters(TWeakPtr<Parameters> InParameters);

private:
	void Invalidate() const;

	/************************************************************************/
	/* event																*/
	/************************************************************************/

private:
	UFUNCTION()
	void OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnClickedCraft();
	
	void OnClickedSort();
};