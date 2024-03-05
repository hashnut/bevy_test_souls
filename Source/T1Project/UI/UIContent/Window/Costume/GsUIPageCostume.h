// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Window/Fairy/GsSelectedPanelInterface.h"
#include "UI/UIContent/Common/Menu/GsUIDropDownMenuEntry.h"
#include "Costume/GsCostumeData.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageUI.h"
#include "DataSchema/UI/GsSchemaWeaponTypeMenu.h"
#include "GsUIPageCostume.generated.h"

class UImage;
class UScrollBox;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;
class UGsUISkillDetailInfo;
class UGsUICostumeDetail;
class UGsUIDropDownMenuCommon;
class UGsUITooltipDesc;

class UTextBlock;
class UGsButton;
class UVerticalBox;
class UWidgetSwitcher;

struct IGsMessageParam;
struct FGsSchemaWeaponTypeMenu;

struct WeaponTypeMenuData : public IMenuData
{
	CreatureWeaponType _weaponType{ CreatureWeaponType::NONE };

	WeaponTypeMenuData(CreatureWeaponType type) : _weaponType(type) {}
};

/**
 * 현재는 하나의 패널 뿐이므로, 지금은 그냥 상속하여 사용 
 * 차후 M4에 TabView Interface와 TabView Selector 제작 예정
 */
UCLASS()
class T1PROJECT_API UGsUIPageCostume : public UUserWidget, public IGsSelectedPanelInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listCostume;

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
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	FGsToggleGroup _toggleGroupGrade;

	TMap<int32, CostumeGrade> _tabGradeIndex;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectGrade;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeDetail* _costumeDetail;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _collectionButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _flavorButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnWearOnOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnWear;
	// 소환 마일리지 팝업 열기 버튼
	// https://jira.com2us.com/jira/browse/CHR-24962
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOpenPopupSummonMileage;
	// 마일리지 이펙트 위젯(받을 보상이 있을때 표시)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _mileageRewardEffectWidget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMenuTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipWeaponMenu;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIDropDownMenuCommon* _weaponSelectMenu;

private:
	// 메시지 바인딩
	TPair<MessageUserInfo, FDelegateHandle> _handlerUserInfo;
	MsgUIHandleArray _uiMsgArray;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnGradeTabChanged(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickDressOn();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickHairOn();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickMenuTooltip();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnOpenCollection();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnOpenFlavorText();

	UFUNCTION()
	void OnCreateCostumeItemEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshCostumeItemEntry(int32 index, UWidget* InEntry);

	UFUNCTION()
	void OnClickOpenPopupSummonMileage();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	bool OnWeaponSelectMenuSelected(int32 index, bool forced = false);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void BeginDestroy() override;

	virtual void InitializeTab(int32 tabIndex) override;
	virtual void ReleasedTab() override;
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	void OnUpdateItemList(TArray<CostumeId>& arr) override;
	
	void OnSelectedCostumeItem(CostumeId id);
	void OnChangeDressUp();

	void OnSetScrollToSelectedCostumeItem(CostumeId id);

protected:
	void SetReserveCostumeData();
	
private:
	void SetRefreshCostumeItems(CostumeGrade grade, CreatureWeaponType selectWeapon = CreatureWeaponType::NONE);
	void UpdateRedDot();
	void SetWeaponSelectMenu();

public:
	CostumeId GetRecoverCostumeId();

private:
	
	TArray<TWeakPtr<FGsCostumeData>> _costumeGradeList;

	int32 _gradeTab = -1;
	CostumeId _selectedCostumeId = INVALID_COSTUME_ID;
	CostumeId _selectedReservedCostumeId = INVALID_COSTUME_ID;
	CreatureGenderType _genderType{ CreatureGenderType::ALL };
	CreatureWeaponType _weaponType{ CreatureWeaponType::ALL };

	CreatureWeaponType _selectedWeapon{ CreatureWeaponType::NONE };

	bool _currentHairOn;

//------------------------------------------------------------------------------------------------------------------
	// 튜토리얼
private:
	int32 _tutorialTargetIndex = -1;

public:
	DECLARE_DELEGATE_OneParam(FOnTutorialSettingComplete, UWidget*);
	FOnTutorialSettingComplete OnTutorialSettingComplete;

public:
	bool IsTutorial() const { return (-1 != _tutorialTargetIndex) ? true : false; }
	void SetTutorialTarget(int32 InIndex);

private:
	UFUNCTION()
	void OnScrollCostumeList(float InCurrentOffset);


	// 소환 마일리지
private:
	// 재화 갱신 메시지
	void OnUpdateCurrency(uint64 InType);
	// 소환 마일리지 데이터 갱신
	void OnChangedSummonMileageData(const  IGsMessageParam* inParam);
	// 마일리지 버튼 상태 갱신
	void SetMileageButtonState();
};
