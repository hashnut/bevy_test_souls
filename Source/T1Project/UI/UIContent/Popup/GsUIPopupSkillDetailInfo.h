// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupSkillDetailInfo.generated.h"

class UTextBlock;
class UGsButton;
class UWidgetSwitcher;
class UGsUISkillDetailInfoDefault;
class UGsUISkillDetailInfoPassivity;
class FGsSkill;


/**
 * 스킬 상세 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSkillDetailInfo : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

	// 0: 스킬일반, 1: 패시비티
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoDefault* _uiDetailInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoPassivity* _uiDetailInfoPassivity;

protected:
	virtual void NativeOnInitialized() override;

protected:
	virtual void OnInputCancel() override;

public:
	// InOverrideLevel 강제로 해당 레벨 정보를 보여줌. 0일경우 FGsSkill의 _level에 따름
	void SetData(SkillId InSkillId, int32 InOverrideLevel = 0);
	void SetData(const FGsSkill* InData, int32 InOverrideLevel = 0);
	void SetPassivityData(PassivityId InPassivityId);

protected:
	UFUNCTION()
	void OnClickClose();
};
