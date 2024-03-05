// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISkillListEntry.generated.h"

class UWidget;
class UImage;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;
class UGsButton;
class UGsUIRedDotBase;
class UGsSwitcherButton;
class UGsUISkillIcon;
class FGsSkillUIData;

/**
 * 스킬 편집 창의 스킬 리스트
 */
UCLASS()
class T1PROJECT_API UGsUISkillListEntry : 
	public UUserWidget, 
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _dimmed;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTreeLineRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgTreeLineLast;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillIcon* _uiSkillIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgEquipMark;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelLevelRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBonusLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelCostume;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCostumeName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
		
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherChain1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherChain2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherChain3;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnAuto;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgCycleSetting;

protected:
	bool _bIsSelected = false;	
	CreatureWeaponType _weaponType = CreatureWeaponType::NONE;
	SkillId _skillId = INVALID_SKILL_ID;
	bool _bIsLearned = false;
	TArray<UWidgetSwitcher*> _switcherChainList;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	void SetData(FGsSkillUIData* InData);

private:
	void SetRedDot(bool bInActive, EGsIconRedDotType InRedDotType);	
	void SetChainStep(int32 InCurrStep, int32 InMaxStep);

private:
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickAuto();
};
