#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "../Fairy/GsSelectedPanelInterface.h"
#include "Management/ScopeGame/GsCostumeManager.h"
//#include "Management/ScopeGame/GsCostumeManager.h"
#include "GsUIPageCostumeSummonConfirm.generated.h"

class UScrollBox;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
class UGsUIWidgetBase;

struct FGsCostumeData;
struct FGsCostumeSummonConfirmData;

UCLASS()
class T1PROJECT_API UGsUIPageCostumeSummonConfirm : public UUserWidget, public IGsSelectedPanelInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listWaitCostume;

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
	TArray<TWeakPtr<FGsCostumeSummonConfirmData>> _costumeData;
	CreatureGenderType _genderType{ CreatureGenderType::ALL };

	TMap<FString, TWeakObjectPtr<UGsUIWidgetBase>> _openPopup;

protected:
	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntry(int32 index, UWidget* entry);

	void OnComfirmSummon(TWeakPtr<FGsCostumeSummonConfirmData> data);
	void OnChangeSummon(TWeakPtr<FGsCostumeSummonConfirmData> data);
	void OnCollectionPopup(CostumeId id);

	void OnFindCostumeInfo(CostumeId id);
	void OnChangeSummonInfoPopup(TWeakPtr<FGsCostumeSummonConfirmData> data);
//	void OnTooltipSkillDetail(TWeakPtr<FGsCostumeSummonConfirmData> data);
	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

	void InitializeTab(int32 tabIndex) override;
	void ReleasedTab() override;
	void OnSelected() override;
	void OnUnselected() override;

	void OnUpdateSummonWaiting(CostumeConfirmId summonWaitDBId);
	void OnConfirmSummonWaiting(CostumeConfirmId summonWaitDBId);
	void OnNotiComfirmChanged();
};