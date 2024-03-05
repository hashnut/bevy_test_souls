// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUI.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillUIData.h"
#include "GsUIWindowSkill.generated.h"

class UPanelWidget;
class UImage;
class UScrollBox;
class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
class UGsSwitcherButton;
class UGsUIRedDotBase;
class UGsUISkillWeaponTab;
class UGsDynamicPanelSlotHelper;
class UGsUITargetDetailInfoBase;
class UGsUISkillDetailInfo;
class UGsUISkillDetailInfoPassivity;
class UGsUISkillDetailInfoCostume;
class UGsUISkillSlotSetEquip;
class UGsUICostumeItem;
class FGsToggleGroup;
class FGsSkill;
struct IGsMessageParam;
struct FGsSchemaSkillSet;

/**
 * 스킬 편집 창
 */
UCLASS()
class T1PROJECT_API UGsUIWindowSkill : public UGsUIWindow
{
	GENERATED_BODY()

public:
	enum class EGsCategoryTabType
	{
		TAB_ACTIVE_SKILL = 0,
		TAB_BUFF_SKILL = 1,
		TAB_PASSIVE_SKILL = 2,
		TAB_COSTUME_SKILL = 3,

		MAX_TYPE
	};

public:	
	using DefCategorySkillDataMap = TMap<SkillCategorySet, TArray<FGsSkillUIData>>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	float _listItemHeight = 160.f;

	// Weapon Tab
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillWeaponTab* _tabCommon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillWeaponTab* _tabSword;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillWeaponTab* _tabDagger;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillWeaponTab* _tabBow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillWeaponTab* _tabShotgun;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillWeaponTab* _tabWand;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillWeaponTab* _tabOrb;	

	// SkillCategorySet Tab
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSkillCategory;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotActive;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotPassive;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotCostume;	

	// SkillList
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollSkillList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSkillList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyList;

	// DetailInfo
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetailInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfo* _uiDetailInfo;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoPassivity* _uiDetailInfoPassivity;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoCostume* _uiDetailInfoCostume;

	// MP 정보 표기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootMPInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMPUsage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMPRecovery;

	// SlotSet	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillSlotSetEquip* _slotSet;

	// Back	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _skillListHelper;

private:
	TMap<CreatureWeaponType, int32> _weaponTabIndexMap;
	TMap<CreatureWeaponType, UGsUISkillWeaponTab*> _weaponTabMap;
	const SkillCategorySet COSTUME_CATEGORY_TYPE = SkillCategorySet::MAX;

	FGsToggleGroup _toggleGroupWeapon;
	FGsToggleGroup _toggleGroupSkillCategory;
	FGsToggleGroup _toggleGroupSkillList;

	MsgGameObjHandleArray _msgHandleGameObjectList;
	MsgUIHandleArray _msgHandleUIList;

	// 관리 편의를 위해 담아서 사용
	TMap<SkillCategorySet, UGsUIRedDotBase*> _redDotCategoryMap;

	int32 _selectedUpgradeStep = 0;
	
	TArray<FGsSkillUIData*> _skillDataList; // 포인터만 가진다
	// 창을 열거나 탭 변경 시 첫 리스트로 자동 선택되어 끄거나 탭 변경 시 레드닷을 꺼야하는 스킬
	SkillId _checkRedDotSkillId = INVALID_SKILL_ID;

	bool _bIsEquipMode = false;
	SkillId _openSelectedSkillId = INVALID_SKILL_ID;

	// 튜토리얼 관련 항목
public:
	DECLARE_DELEGATE_OneParam(FOnTutorialSkillListEvent, UWidget* /*TargetWidget*/);
	FOnTutorialSkillListEvent OnTutorialSkillListEvent;

private:
	UWidget* _tutorialTarget = nullptr;
	SkillId _tutorialSkillId = INVALID_SKILL_ID;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool SyncContentState() override;

	virtual void InvalidateAllInternal() override;

public:
	virtual void Close(bool bImmediate = false, bool bRecoverPrevStack = true) override;

private:
	void BindMessages();
	void UnbindMessages();

private:
	void InvalidateSkillList(SkillId InSelectedSkillId = INVALID_SKILL_ID);
	void InvalidateSlotSet(bool bInForced = false);
	void InvalidateSkillListTarget(SkillId InSkillId); // 스킬리스트의 특정 항목만 갱신
	void InvalidateSlotEnableEquipState();
	void InvalidateMPInfo();
	
	// 0: 슬롯 장착 / 1: 미리보기
	//void SetUIEquipMenu(int32 InIndex, bool bInvalidate);
	void SetEquipMark(CreatureWeaponType InType);
	void SetEquipMode(bool bIsEquipMode, bool bIsForced = false);

private:
	void OnClickTabWeapon(int32 InIndex);
	void OnClickTabSkillCategory(int32 InIndex);
	void OnSelectSkillList(int32 InIndex);
	void OnClickSkillList(int32 InIndex, bool bIsSelected);
	void OnClickSkillSlot(class UGsUISkillSlotEquip* InSlot);	
	void OnClickEnchantOrLearn(bool bIsEnchant); // T: 강화, F: 습득
	void OnClickEquipOrCancel(bool bIsEquipMode); // T: 장착, F: 취소 
	void OnClickUpgradeStep(uint8 InUpgradeStep); // 상세창 업그레이드 각 단계 슬롯 클릭
	void OnClickRemoveEquip(); // 슬롯 해제 클릭
	void OnClickCostumeLearn(bool);
	void OnClickCostumeEquip(bool);
	void OnClickCostumeRemoveEquip();
	void OnClickCycleSetting();

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);
	
private:
	void OnChangeSkillSlotEquip(const IGsMessageParam* InParam);
	void OnChangeSkillSlotAuto(const IGsMessageParam* InParam);
	void OnChangeSkillBonusLevel(const IGsMessageParam* InParam);
	void OnChangeSkillCycle(const IGsMessageParam* InParam);
	void OnSkillUpdateRedDot(const IGsMessageParam*);
	void OnSkillEnchantResult(const IGsMessageParam* InParam);

private:
	int32 ConvertWeaponTypeToIndex(CreatureWeaponType InType) const;
	CreatureWeaponType ConvertIndexToWeaponType(int32 InIndex) const;

	bool CanEquipSkill(const FGsSkill* InSkill, bool bShowMessage = false) const;
	void SortSkillList(OUT TArray<FGsSkillUIData*>& OutList);
	bool SortSkillData(const FGsSchemaSkillSet* InA, const FGsSchemaSkillSet* InB);
	void ScrollSkillListToTarget(int32 InIndex);

	void RequestSetSkillSlot(SkillSlotId InSlotId, FGsSkillUIData* InSkillData);

	void SetSkillRedDotOff(const FGsSkill* InSkill, bool bUpdateUI);
	void UpdateRedDot(bool bAllTab);
	void UpdateRedDotInter(CreatureWeaponType InType, OUT bool& bOutRedDot, OUT bool& bOutReinforce);
		
	void PlaySkillPreview(FGsSkillUIData* InData);

	// -1이면 자체 체크해서 설정
	int32 GetDetailInfoPageIndex(int32 InCategoryIndex = -1) const;

private:
	FGsSkillUIData* GetSelectedSkillData() const;
	SkillId GetSelectedSkillId() const;
	const FGsSkill* GetSelectedSkill() const;
	CreatureWeaponType GetSelectedWeaponType() const;	
	SkillCategorySet GetSelectedSkillCategorySet() const;

	// 업그레이드 스킬까지 전부 검색
	const FGsSkillUIData* FindUpgradeSkill(const FGsSkill* InSkill);
	bool IsCostumeCategory() const;
	UGsUITargetDetailInfoBase* GetCurrentDetailInfo();

	// 튜토리얼 관련 항목
public:
	void SetTutorial(const FGsSkill* InSkill);
	UWidget* GetFirstSkillSlot() const;

private:
	UWidget* GetTargetSkillListItem(SkillId InSkillId) const;
	void SetTutorialTarget(UWidget* InWidget);
	bool IsTutorial() const { return (INVALID_SKILL_ID != _tutorialSkillId) ? true : false; }

	UFUNCTION()
	void OnScrollSkillList(float InCurrentOffset);
};
