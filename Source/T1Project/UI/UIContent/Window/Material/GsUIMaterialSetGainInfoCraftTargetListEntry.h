#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Item/Collection/GsItemCollectionCondition.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUIMaterialSetGainInfoCraftTargetListEntry.generated.h"

class FGsCostPackage;
class UGsButton;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsUIIconBase;
class UGsUIIconItem;
class UGsWrapBoxIconSelector;
class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class UGsUIMaterialSetGainInfoCraftTargetListEntry final : public UUserWidget, public IGsEntryInterface
{

	GENERATED_BODY()

public:
	struct Parameters final
	{

		friend class UGsUIMaterialSetGainInfoCraftTargetListEntry;

	private:
		const FGsItemCollectionCondition* _collectionCondition = nullptr;

	public:
		Parameters() = default;

		explicit Parameters(const FGsItemCollectionCondition& InCollectionCondition)
			: _collectionCondition(&InCollectionCondition)
		{
		}

	private:
		/** raw pointer 멤버인 _collectionCondition 의 생명 유지 기간은, 최초 호출될 때 외에는 접근 가능 여부를 확정할 수 없음.
		 * 이 함수 1회 호출 시, 저장된 CollectionCondition 를 반환 후 raw pointer 를 삭제함.
		 * 이 함수는 가능한 매개변수 전달과 동일한 틱 내에서만 사용할 것을 추천.
		 */
		const FGsItemCollectionCondition* PopCollectionCondition()
		{
#if WITH_EDITOR
			if (nullptr == _collectionCondition)
			{
				GSLOG(Warning, TEXT("nullptr == _collectionCondition"));
			}
#endif

			const FGsItemCollectionCondition* CollectionCondition = _collectionCondition;
			_collectionCondition = nullptr;
			return CollectionCondition;
		}

	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _targetIconSelector;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UGsDynamicIconSlotHelper* _targetSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _materialIconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UGsDynamicIconSlotHelper* _materialSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _detailInfoSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCraft;

private:
	TWeakPtr<Parameters> _parameters;

	bool _collectionCompleted;

	CraftId _craftId;

	ItemId _itemId;

	Level _itemLevel;

protected:
	void NativeOnInitialized() override;

	void NativeDestruct() override;

	/************************************************************************/
	/* setter																*/
	/************************************************************************/

public:
	/**
	 * 매개변수 내부 멤버가 raw pointer 이므로 생명 유지 기간을 확정할 수 없음.
	 */
	void SetParameters(TWeakPtr<Parameters> InParamerters);

	/************************************************************************/
	/* event																*/
	/************************************************************************/

private:
	UFUNCTION()
	void OnRefreshIconTarget(int32 InIndex, UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnRefreshIconMaterial(int32 InIndex, UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnClickedCraft();

	void OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon);

};
