// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Skill/GsUITargetDetailInfoBase.h"
//#include "UTIL/GsIntervalChecker.h"
#include "GsUISkillDetailInfoCostume.generated.h"

class UWidgetSwitcher;
class UGsButton;
class UGsUISkillDetailInfoDefault;
/**
 * 스킬 상세정보 UI - 스킬창 의 패시비티
 */
UCLASS()
class T1PROJECT_API UGsUISkillDetailInfoCostume : public UGsUITargetDetailInfoBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FGsOnClickActionButton, bool /*bIsAction*/);
	DECLARE_DELEGATE(FGsOnClickNoParam);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoDefault* _detailInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLearn;

	// 주의: 0:장착 1:해제
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherEquip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEquip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRemoveEquip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCycleSetting;

public:
	// 강화 없으므로 무조건 false로 보냄
	FGsOnClickActionButton OnClickEnchantOrLearn; // T: 강화 F: 습득
	// 코스튬 슬롯은 무조건 장착이므로 true만 보냄
	FGsOnClickActionButton OnClickEquipOrCancel; // T: 장착 F: 취소

	FGsOnClickNoParam OnClickRemoveEquip; // 슬롯해제
	FGsOnClickNoParam OnClickSkillCycleSetting; // 스킬 주기 설정

public:
	virtual void NativeOnInitialized() override;

public:
	virtual void SetDataAll(FGsSkillUIData* InData) override;
	virtual void SetData(const FGsSkill* InData, int32 InOverrideLevel = 0) override;
	virtual void SetPassivityData(PassivityId InPassivityId) override;

private:
	UFUNCTION()
	void OnClickLearn();	
	UFUNCTION()
	void OnClickEquip();
	UFUNCTION()
	void OnClickRemoveFromSlot();
	UFUNCTION()
	void OnClickCycleSetting();
};
