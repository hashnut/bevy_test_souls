// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "SharedEnums/SharedFairyEnum.h"
#include "GsSelectedPanelInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Tray/FairyEnchant/GsEnchantUITaskControl.h"
#include "UI/UIContent/Window/Fairy/GsUIFairySkillItem.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageUI.h"
#include "GsUIPageFairy.generated.h"

class UImage;
class UTextBlock;
class UScrollBox;
class UWidgetAnimation;
class UPanelWidget;
class UWidgetSwitcher;

class UGsButton;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsUISkillDetailInfo;

class UGsUIFairyStat;
class UGsUIFairySkillItem;
class UGsUIIconBase;
class UGsDivFairyEnchant;
class UGsUIIconFairyEnchant;

class FGsItem;
struct FGsFairyData;
struct FGsSchemaPassivitySet;
struct IGsMessageParam;

/**
 * 현재는 하나의 패널 뿐이므로, 지금은 그냥 상속하여 사용 
 * 차후 M4에 TabView Interface와 TabView Selector 제작 예정
 */
UCLASS()
class T1PROJECT_API UGsUIPageFairy : public UUserWidget, public IGsSelectedPanelInterface
{
	GENERATED_BODY()

public:
	enum class ActTaskState : uint8 { None, Info, Enchant };

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeMagic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeRare;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeEpic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeUnique;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeLegend;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotMagic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotRare;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotEpic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotUnique;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotLegend;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotAll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	FGsToggleGroup _toggleGroupGrade;

	TMap<int32, FairyGrade> _tabGradeIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryFairySkillClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _skillPanelHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _fairyName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _fairyLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textFairyGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorTextGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _statListPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryItemStat;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _statScrollHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _skill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _fairySkillBox;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIFairySkillItem* _fairySkill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIFairySkillItem* _fairyVehicleSkill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSummon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFlavor;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEnchant;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsDivFairyEnchant* _fairyEnchant;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _fairyEnchantSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _materialPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIIconFairyEnchant* _resIcon;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//FSlateBrush _brush;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _levelImageSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorItemCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemRequireCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textProbability;

	// 소환 마일리지 팝업 열기 버튼
	// https://jira.com2us.com/jira/browse/CHR-24962
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOpenPopupSummonMileage;
	// 마일리지 이펙트 위젯(받을 보상이 있을때 표시)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _mileageRewardEffectWidget;

protected:
	FWidgetAnimationDynamicEvent _timerOnAnimationEnd;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _animationEnchant;

private:
	struct FairyStatInfo
	{
		FairyLevelId _level;
		TPair<FText, FText> _statText;
		bool _isLock;
	};

	TArray<TWeakPtr<FGsFairyData>> _fairyGradeList;
	TArray<FairyStatInfo> _statTxtList;
	TArray<TSharedPtr<FGsFairyPassivityInfo>> _fairyLevelData;
	int32 _gradeTab{ -1 };
	FairyId _selectedFairyId{ INVALID_FAIRY_ID };

	TSharedPtr<FGsItem> _enchantItem;
	
	void SetReserveActionByItem();
private:
	FairyId _selectReserveFairyId{ INVALID_FAIRY_ID }; //collection 에서 호출하는 id
	
	int _changedState{ 0 };
	ActTaskState _curState{ ActTaskState::None };
	FGsTaskStateManager<ActTaskState> _stateManager;

private:
	FGsTaskManager _taskManager;
	uint64 _effectSoundID[(uint8)EffectSoundResType::Sound_Max];

	//------------------------------------------------------------------------------------------------------------------
	// 튜토리얼
private:
	int32 _tutorialTargetIndex = -1;

public:
	DECLARE_DELEGATE_OneParam(FOnTutorialSettingComplete, UWidget*);
	FOnTutorialSettingComplete OnTutorialSettingComplete;

private:
	// 메시지 바인딩
	TPair<MessageUserInfo, FDelegateHandle> _handlerUserInfo;
	MsgUIHandleArray _uiMsgArray;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnGradeTabChanged(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSummon();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnOpenCollection();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnOpenFlavorText();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnChangeEnchantMode();

	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntry(int32 index, UWidget* InEntry);
	
	UFUNCTION()
	void OnRefreshStatEntry(int32 index, UWidget* InEntry);

	UFUNCTION()
	void OnCreateSkillEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshSkillEntry(int32 index, UWidget* InEntry);

	UFUNCTION()
	void OnStateChaneAnimationFinished();

	UFUNCTION()
	void OnPressedLongIcon();

	void OnTooltipFairySkill(TWeakPtr<FGsFairyPassivityInfo> info);
	void OnTooltipVehicleFairySkill(FairyLevelId level);

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void InitializeTab(int32 tabIndex) override;
	void ReleasedTab() override;
	void OnSelected() override;
	void OnUnselected() override;
	void OnUpdateItemList(TArray<FairyId>& arr) override;

	void OnSelectedFairyItem(FairyId id);
	void OnChangeSummonFairy();

	void OnEnchantFairyResult(FairyId id, FairyEnchantResult result);

	void OnSetScrollToSelectedFairyItem(FairyId id);

protected:
	void OnStateFairyInfo();
	void OnStateFairyEnchant();

	void SetSelectedFairyUpgradeInfo(FairyId fairyId);
	void SetReserveFairyData();

	void OnStartFairyEnchant(TWeakPtr<FGsFairyData> enchantFairy);
	void OnCancelFairyEnchant();

private:
	void SetRefreshFairyItems(FairyGrade grade, ActTaskState state = ActTaskState::None);
	void UpdateItemSelection(FairyId selectId);
	void UpdateRedDot();

	void EnchantProcess(FairyId useID);
	void CancelEnchant();

protected:
	virtual void PlayEffectSound(EffectSoundResType InResType) override;
	virtual void StopEffectSound(EffectSoundResType InResType) override;

public:	
	bool IsTutorial() const { return (-1 != _tutorialTargetIndex) ? true : false; }
	void SetTutorialTarget(int32 InIndex);

private:
	UFUNCTION()
	void OnScrollFairyList(float InCurrentOffset);

	UFUNCTION()
	void OnClickOpenPopupSummonMileage();

	// 소환 마일리지
private:
	// 재화 갱신 메시지
	void OnUpdateCurrency(uint64 InType);
	// 소환 마일리지 데이터 갱신
	void OnChangedSummonMileageData(const  IGsMessageParam* inParam);
	// 마일리지 버튼 상태 갱신
	void SetMileageButtonState();
};
