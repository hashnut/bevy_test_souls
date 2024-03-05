// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Skill/GsUISkillDetailInfoBase.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUISkillDetailInfo.generated.h"

class FGsSkill;
class UPanelWidget;
class UScrollBox;
class UGsButton;
class UTextBlock;
class URichTextBlock;
class UGsUISkillSlotEquipStep;
class FGsSkillUIData;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;

/**
 * 스킬 상세정보 UI - 스킬창
 */
UCLASS()
class T1PROJECT_API UGsUISkillDetailInfo : public UGsUISkillDetailInfoBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FGsOnClickActionButton, bool /*bIsAction*/);
	DECLARE_DELEGATE_OneParam(FGsOnClickUpgradeStep, uint8 /*upgradeStep*/);
	DECLARE_DELEGATE(FGsOnClickNoParam);

public:
	enum EGsSkillEnchantBtnState
	{
		LEARN = 0,
		ENCHANT,
	};

	enum EGsSkillEquipBtnState
	{
		EQUIP = 0,
		CANCEL,
		REMOVE,	
	};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _slotWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _rbRequiredPassivity;

	// 레벨, 스킬타입과 단계
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootLevelAndType;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBonusLevel;
	
	// 업그레이드 스킬 단계표시 (구현을 갈라야될것 같기도)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootUpgradeStep;
	
	// 버튼 0: 습득 1: 강화
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherEnchant;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLearn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEnchant;

	// 주의: 0:장착 1:취소 2:해제
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherEquip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStartEquipMode;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancelEquipMode;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRemoveEquip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCycleSetting;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	FText _textFormatNStep;

	// R2: 스킬 튜토리얼에서 습득하자마자 광클 시 습득 버튼을 눌러 강화창이 열리므로 두 버튼이 공유함
	FGsIntervalChecker _checkerButton; 

	// 갱신할때만 채운다. 쓰레기 포인터를 저장할까봐 가능하면 포인터 캐싱을 막는다.
	TArray<TWeakPtr<const FGsSkill>> _tempUpgradeSkillList;
	// 현재 업그레이드 스킬
	SkillId _currUpgradeSkillId = INVALID_SKILL_ID;

public:
	FGsOnClickActionButton OnClickEnchantOrLearn; // T: 강화 F: 습득
	FGsOnClickActionButton OnClickEquipOrCancel; // T: 장착 F: 취소
	FGsOnClickUpgradeStep OnClickUpgradeStep; // 각 단계 슬롯 클릭
	FGsOnClickNoParam OnClickRemoveEquip; // 슬롯해제
	FGsOnClickNoParam OnClickSkillCycleSetting; // 스킬 주기 설정

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:	
	virtual void SetData(const FGsSkill* InData, int32 InOverrideLevel = 0) override;
	virtual void SetEquipMode(bool bIsEnableEquip, bool bIsEquipMode) override;

public:
	void SetDataAll(FGsSkillUIData* InData);

private:
	UFUNCTION()
	void OnCreateEntry(class UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickLearn();
	UFUNCTION()
	void OnClickEnchant();
	UFUNCTION()
	void OnClickStartEquipMode();
	UFUNCTION()
	void OnClickCancelEquipMode();
	UFUNCTION()
	void OnClickRemoveFromSlot();
	UFUNCTION()
	void OnClickCycleSetting();
	
	void OnClickSlotStep(UGsUISkillSlotEquipStep* InSlot);

private:
	void SetUpgradeStep(const FGsSkillUIData* InData);
	void SelectUpgradeStep(uint8 InIndex);
	void SetRequiredPassivity(const FGsSkill* InData);

	void SetGradeTag(ItemGrade InGrade, const FText& InTargetText, OUT FText& OutText);
};
