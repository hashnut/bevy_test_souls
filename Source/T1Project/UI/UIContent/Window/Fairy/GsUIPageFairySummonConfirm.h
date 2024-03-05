#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsSelectedPanelInterface.h"
#include "GsUIPageFairySummonConfirm.generated.h"

class UScrollBox;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
class UGsUIWidgetBase;

struct FGsFairyData;
struct FGsFairySummonConfirmData;

UCLASS()
class T1PROJECT_API UGsUIPageFairySummonConfirm : public UUserWidget, public IGsSelectedPanelInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listWaitFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textCount;

protected:
	TArray<TWeakPtr<FGsFairySummonConfirmData>> _fairyData;

	TMap<FString, TWeakObjectPtr<UGsUIWidgetBase>> _openPopup;

protected:
	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntry(int32 index, UWidget* entry);

	void OnComfirmSummon(TWeakPtr<FGsFairySummonConfirmData> data);
	void OnChangeSummon(TWeakPtr<FGsFairySummonConfirmData> data);
	void OnCollectionPopup(FairyId id);
	void OnFindFairyInfo(FairyId id);
	void OnChangeSummonInfoPopup(TWeakPtr<FGsFairySummonConfirmData> data);
	void OnTooltipSkillDetail(TWeakPtr<FGsFairySummonConfirmData> data);
	void OnTooltipVehicleSkillDetail(TWeakPtr<FGsFairySummonConfirmData> data);

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

	void InitializeTab(int32 tabIndex) override;
	void ReleasedTab() override;
	void OnSelected() override;
	void OnUnselected() override;

	void OnUpdateSummonWaitingFairy(FairyConfirmId summonWaitDBId);
	void OnConfirmSummonWaitingFairy(FairyConfirmId summonWaitDBId);

	void OnNotiComfirmChanged();
};