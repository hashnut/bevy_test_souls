#pragma once

#include "UI/UILib/Base/GsUIPopup.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupItemCollectionAlternateList.generated.h"

class UGsButton;
class UScrollBox;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGsUIPopupItemCollectionAlternateList : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApply;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxItemList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperItemList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfItem;

	const class FGsItemCollectionCondition* _data = nullptr;

	ItemId _selectedMaterialId = INVALID_ITEM_ID;

public:
	void BeginDestroy() override;
protected:
	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;
		
protected:
	UFUNCTION()
	void OnSelectedDelegate(UUserWidget* InEntry);

	UFUNCTION()
	void OnClickApply();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshItemList(int32 InIndex, UWidget* InEntry);

protected:
	void SetSelected(ItemId InMaterialId);

public:
	void SetData(const class FGsItemCollectionCondition* InData);
};