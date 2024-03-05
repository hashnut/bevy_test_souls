#pragma once

#include "CoreMinimal.h"
#include "GSUIMonsterCollectionBasePage.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "GSUIMonsterCollectionSlatePage.generated.h"


class UGsButton;
class UScrollBox;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGSUIMonsterCollectionSlatePage : public UGSUIMonsterCollectionBasePage
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxCategory;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCategory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCategoryEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxSlateSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSlateSet;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSlateSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfSlateEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetailPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFilter;

protected:
	TArray<EMonsterKnowledgeSlateCategory> _categorySet;

	FGsToggleGroup _toggleGroupCategoryTab;
	FGsToggleGroup _toggleGroupSlateSet;	

	int32 _prevSelectedSlateIndex = 0;
	float _prevScrollValue = 0.f;

	int32 _prevSelectedNodeId = 0;
	bool _prevCheckedNodeAll = false;
	int32 _prevSelectedSpecialNodeId = 0;

	uint32 _reservedId = 0;

	TArray<StatType> _prevStatFilterSet;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
public:
	virtual void Enter(struct FGsMonsterKnowledgeUICaptureData* InData) override;
	virtual void Leave() override;
	virtual void Capture(struct FGsMonsterKnowledgeUICaptureData* OutData) override;
	virtual bool CloseChildWidget() override;

	virtual void InvalidateSlate(MonsterKnowledgeSlateId InId) override;
	virtual void InvalidateSlateCurrency() override;

protected:
	virtual void InvalidateRedDot() override;
	virtual void BuildData(int32 InCategoryIndex) override;

protected:
	UFUNCTION()
	void OnSelectedCategoryTab(int32 InIndex);

	UFUNCTION()
	void OnSelectedSlate(int32 InIndex);

	UFUNCTION()
	void OnRefreshCategoryTab(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnRefreshSlateSet(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickedOpenFilter();

	void OnCallbackStatFilterApply(const TArray<StatType>& InSelectedStatSet);

private:

	//	//----------------------------------------------------------------------------------------------------------------------
	//		// Tutorial
	//private:
	//	bool _bIsTutorial = false;
	//
	//public:
	//	void SetTutorial(bool bIsTutorial) { _bIsTutorial = bIsTutorial; }
	//	bool IsTutorial() const { return _bIsTutorial; }
	//
	//	DECLARE_DELEGATE_OneParam(FOnTutorialSettingComplete, UWidget*);
	//	FOnTutorialSettingComplete OnTutorialSettingComplete;
	//
	//	void OnTutorialListSetting(UWidget* InWidget);
};
