// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupSkillEnchant.generated.h"

class UTextBlock;
class UScrollBox;
class UPanelWidget;
class UGsButton;
class UGsUICurrencyButton;
class UGsUIChangeStatGroup;
class UGsUISkillIcon;
class UGsDynamicPanelSlotHelper;
class FGsSkill;
struct FGsSchemaSkillLevelInfo;
struct FGsSchemaSkillUpCostSet;
struct FGsSchemaSkillUpgradeInfo;
struct FGsSchemaSkillLevelSet;


/**
 * 스킬 강화 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSkillEnchant : public UGsUIPopup
{
	GENERATED_BODY()

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

	// 현재 스킬 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillIcon* _uiSkillIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEffect;

	// 다음 스킬 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevelNext;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameNext;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillIcon* _uiSkillIconNext;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEffectNext;

	// 스탯 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxEffect;

	// 재료 표시
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollIngredients;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIngredients;

	// 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _enchantBtnUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnProbability;
	
protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperIngredients;

protected:
	SkillId _skillId = INVALID_SKILL_ID;
	const FGsSchemaSkillUpCostSet* _costTable = nullptr; // 강화/업그레이드에 필요한 아이템, 재화 정보	

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	virtual void InvalidateAllInternal() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetData(const FGsSkill* InData);

protected:
	void InvalidateUI(const FGsSkill* InData);	

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickEnchant();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickProbability();

private:
	bool CheckEnableEnchant(bool bInShowTicker);

private:
	void SetUISkillDefaultInfo(const FGsSkill* InSkill, int32 InLevel, 
		OUT UGsUISkillIcon* OutIcon, OUT UTextBlock* OutTbLevel, OUT UTextBlock* OutTbName);
	void SetUIEffectList(SkillId InSkillId, const FGsSchemaSkillLevelInfo* InLevelInfo,
		SkillId InSkillIdNext, const FGsSchemaSkillLevelInfo* InLevelInfoNext);

	void AppendCoolTime(float InCoolTime, OUT FString& OutData);
	void AppendConsumeInfo(const FGsSchemaSkillLevelInfo* InInfo, OUT FString& OutData);
	bool IsSameConsumeInfo(const FGsSchemaSkillLevelInfo* InCurr, const FGsSchemaSkillLevelInfo* InNext);
};
