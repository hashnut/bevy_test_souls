#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "../Fairy/GsSelectedPanelInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPageCostumeCollection.generated.h"

/**
 *
 */
class UGsUIListCostumeCollectionData;
class UGsToggleButton;
class UGsButton;
class UListView;
class UTextBlock;
class UPanelWidget;
class FGsCostumeCollection;

UCLASS()
class T1PROJECT_API UGsUIPageCostumeCollection : public UUserWidget, public IGsSelectedPanelInterface
{
	GENERATED_BODY()

	const static int32 PAGE_OVER_COUNT = 10;

protected:
	const int32 ViewItemCount = 10;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewCollections;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleComplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleIncomplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCollectionEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPrev;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNext;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textPage;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyPanel;

	UFUNCTION(BlueprintCallable)
	void OnPrev();

	UFUNCTION(BlueprintCallable)
	void OnNext();

	UFUNCTION(BlueprintCallable)
	void OnClickProgressDetail();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelectedTypeChanged(int32 index);

	UFUNCTION(BlueprintCallable)
	void OnClickStatFilter();

	FGsToggleGroup _toggleGroupType;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void BeginDestroy() override;

	void InitializeTab(int32 tabIndex) override;
	void ReleasedTab() override;
	void OnSelected() override;
	void OnUnselected() override;
	void OnUpdateItemList(TArray<CostumeId>& arr) override;

	bool CloseChildWidget();
private:
	void SetRefreshCostumeItems();
	void UpdateRedDot();

	void OnPopupCostumeEachCollection(CostumeId id);
	void SetIntoView(int32 InIndex);
	void SetListCountDisplay(int32 InIndex);

	void OnCallbackStatFilterApply(const TArray<StatType>& InSelectedStatSet);
private:
	int32 _currentCount;

	TWeakPtr<FGsCostumeCollection> _selectedCollection;
	int8 _selectedType{ -1 };
	CreatureGenderType _genderType{ CreatureGenderType::ALL };

	TArray<StatType> _selectedStatSet;
};