#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Blueprint/UserWidget.h"
#include "GsSelectedPanelInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPageFairyCompose.generated.h"

class UImage;
class UScrollBox;
class UTextBlock;
class UPanelWidget;
class UGsButton;
class UGsSwitcherButton;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;

class UGsUISkillDetailInfo;
class UGsUIFairyStat;
class UGsUIFairyItem;
class UGsUIFairyComposeRes;
class UGsUIFairyComposeResEffect;

struct FGsFairyData;

UCLASS()
class T1PROJECT_API UGsUIPageFairyCompose 
    : public UUserWidget, public IGsSelectedPanelInterface
{
    GENERATED_BODY()
public:
	enum class ActTaskState : uint8 { None, Composition, Ceiling };

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeMagic;

	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeRare;

	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeEpic;

	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeUnique;

	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeLegend;

	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UWidgetSwitcher* _decoSelectGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UWidgetSwitcher* _composeGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUIFairyItem* _ItemFairyPortrait1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUIFairyItem* _ItemFairyPortrait2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUIFairyItem* _ItemFairyPortrait3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUIFairyItem* _ItemFairyPortrait4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUIFairyItem* _ItemFairyPortrait5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUIFairyItem* _ItemFairyPortrait6;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIFairyComposeRes* _ItemFairyComposeResult;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _bonusRate;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBonusRate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _multiCompose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMultiComposeCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoAdd;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCompose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCeiling; //test

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _lockRes5;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switchLockRes5;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _lockRes6;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switchLockRes6;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIFairyComposeResEffect* _composeEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFailureInfo;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	FGsToggleGroup _toggleGroupGrade;
	TMap<int32, FairyGrade> _tabGradeIndex;

private:
	TArray<TPair<int, UGsSwitcherButton*>> _addRessources;
	TArray<FairyGrade> _enableCeilingGrade;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnGradeTabChanged(int32 index);

	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntry(int32 index, UWidget* entry);

	UFUNCTION()
	void OnAutoResourceAdd();

	UFUNCTION()
	void OnStartComposition();

	UFUNCTION()
	void OnPopupCeilingSummon(); //test

	UFUNCTION()
	void OnAcqFailureItem();

	UFUNCTION()
	void OnClickResource5Toggle();

	UFUNCTION()
	void OnClickResource6Toggle();

public:
	void InitializeTab(int32 tabIndex) override;
	void ReleasedTab() override;
	void OnSelected() override;
	void OnUpdateItemList(TArray<FairyId>& arr) override;

	void OnSummonComplete();

	void OnComposeFairyResult(TArray<FairyId>& dataList);
	void OnCeilingFairyResult(bool success);

	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

private:
	void InitUI();

	void SetRefreshFairyItems(FairyGrade grade, ActTaskState state = ActTaskState::None);
	void SelectAutoComposition();
	void ClearCompositionResourceItem();

	int32 SetResourceToUsableSlot(FairyId id);

	void StartComposition();
	void StartCeiling(uint32 gradeIndex, uint8 phase);
	void CancelCeiling();
	
protected:
	void OnSelectedFairyItem(FairyId id);
	void OnSelectedResourceItem(int8 portraitindex, FairyId id);

	void OnStateFairyCompose();
	void OnStateFairyCeiling();

protected:
	virtual void PlayEffectSound(EffectSoundResType InResType) override;
	virtual void StopEffectSound(EffectSoundResType InResType) override;
	
private:
	static constexpr int maxCompositionPanel = 11;
	static constexpr int maxResCount = 6;
	static constexpr int minResCount = 4;
	struct ComposeGradeData
	{
		int32 _costGold{ 0 };
		FairyGrade _successGrade{ FairyGrade::NONE };
		float _upgradeRate[maxResCount + 1]{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	};

	TMap<FairyGrade, ComposeGradeData> _gradeData;

	FairyGrade _selectedFairyGrade{ FairyGrade::NONE };
	FString _multiComposeMsg;

	TArray<UGsUIFairyItem*> _composeFairyPortrait;

	struct CompositionPan
	{
		FairyId _list[maxResCount]{ 0, 0, 0, 0, 0, 0 };
	};

	TArray<CompositionPan> _selectFairyIdList;

	TArray<TWeakPtr<FGsFairyData>> _fairyGradeList;

	int _gradeTab = -1;
	TMap<FairyGrade, UGsSwitcherButton*> _gradeTabButtons;

private:
	uint64 _effectSoundID[(uint8)EffectSoundResType::Sound_Max];

	bool _lockState{ false };
	ActTaskState _curState{ ActTaskState::None };
	FGsTaskStateManager<ActTaskState> _stateManager;

	FGsTaskManager _taskManager;
};

