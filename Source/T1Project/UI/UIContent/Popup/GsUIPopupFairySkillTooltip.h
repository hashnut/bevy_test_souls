// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "GsUIPopupFairySkillTooltip.generated.h"

class UTextBlock;
class UGsButton;
class UScrollBox;
class UPanelWidget;
class UWidgetSwitcher;
class UGsUIPassivityDetailInfo;
class UGsUISummonPortraitBase;
struct FGsFairyData;
struct FGsSchemaPassivitySet;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupFairySkillTooltip : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _fairyName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISummonPortraitBase* _portrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPrev;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNext;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textSkillName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textSkillLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPassivityDetailInfo* _uiDetailInfo;	
	
	TArray<TPair<FairyLevelId, const FGsSchemaPassivitySet*>> _skillPassivityList;

	FairyLevelId _curLevel{ INVALID_FAIRY_LEVEL_ID };
	FairyLevelId _maxLevel{ INVALID_FAIRY_LEVEL_ID };
	FairyLevelId _minLevel{ INVALID_FAIRY_LEVEL_ID };

public:
	struct PopupInitData
	{
		TWeakPtr<FGsFairyData> data;
		TArray<TPair<FairyLevelId, const FGsSchemaPassivitySet*>> skillPassivityList;
		FairyLevelId viewLevel{ INVALID_FAIRY_LEVEL_ID };
		bool isNoneLevelSkill{ false };
	};


protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void OnClickClose();

	UFUNCTION(BlueprintCallable)
	void OnClickNextLevel();

	UFUNCTION(BlueprintCallable)
	void OnClickPrevLevel();

public:
	void InitializeData(PopupInitData* initParam = nullptr);

protected:
	void SetFairyInfo(TWeakPtr<FGsFairyData> data);
	void SetData(FairyLevelId level, bool noneLevelSkill = false);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
