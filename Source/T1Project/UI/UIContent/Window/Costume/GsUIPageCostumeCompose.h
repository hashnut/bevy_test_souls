#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Window/Fairy/GsSelectedPanelInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPageCostumeCompose.generated.h"

class UImage;
class UScrollBox;
class UTextBlock;
class UGsButton;
class UGsSwitcherButton;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;

class UGsUISkillDetailInfo;
class UGsUICostumeItem;
class UGsUIFairyComposeRes;
class UGsUIFairyComposeResEffect;

struct FGsCostumeData;

UCLASS()
class T1PROJECT_API UGsUIPageCostumeCompose
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
	UScrollBox* _listCostume;

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
	UGsUICostumeItem* _ItemCostumePortrait1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUICostumeItem* _ItemCostumePortrait2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUICostumeItem* _ItemCostumePortrait3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUICostumeItem* _ItemCostumePortrait4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUICostumeItem* _ItemCostumePortrait5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIResource", meta = (BindWidget))
	UGsUICostumeItem* _ItemCostumePortrait6;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIFairyComposeRes* _ItemComposeResult;

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
	TMap<int32, CostumeGrade> _tabGradeIndex;
private:
	TArray<TPair<int, UGsSwitcherButton*>> _addRessources;

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
	void OnUpdateItemList(TArray<CostumeId>& arr) override;

	void OnSummonComplete();

	void OnComposeResult(TArray<CostumeId>& dataList);
	void OnCeilingResult(bool success);

	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

private:
	void InitUI();

	void SetRefreshCostumeItems(CostumeGrade grade);
	void SelectAutoComposition();
	void ClearCompositionResourceItem();

	int32 SetResourceToUsableSlot(CostumeId id);

	void StartComposition();
	void StartCeiling(uint32 gradeIndex, uint8 phase);
	void CancelCeiling();

protected:
	void OnSelectedCostumeItem(CostumeId id);
	void OnSelectedResourceItem(int8 portraitindex, CostumeId id);

	void OnStateCostumeCompose();
	void OnStateCostumeCeiling();

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
		CostumeGrade _successGrade{ CostumeGrade::NONE };
		float _upgradeRate[maxResCount + 1]{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	};

	TMap<CostumeGrade, ComposeGradeData> _gradeData;

	CostumeGrade _selectedGrade{ CostumeGrade::NONE };
	FString _multiComposeMsg;

	TArray<UGsUICostumeItem*> _composeCostumePortrait;

	struct CompositionPan
	{
		CostumeId _list[maxResCount]{ 0, 0, 0, 0, 0, 0 };
	};

	TArray<CompositionPan> _selectCostumeIdList;

	TArray<TWeakPtr<FGsCostumeData>> _gradeList;
	TArray<CostumeGrade> _enableCeilingGrade;

	int _gradeTab = -1;
	TMap<CostumeGrade, UGsSwitcherButton*> _gradeTabButtons;

	CreatureGenderType _genderType{ CreatureGenderType::ALL };

private:
	uint64 _effectSoundID[(uint8)EffectSoundResType::Sound_Max];

	bool _lockState{ false };
	ActTaskState _curState{ ActTaskState::None };
	FGsTaskStateManager<ActTaskState> _stateManager;

	FGsTaskManager _taskManager;
};