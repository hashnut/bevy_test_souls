// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Skill/GsUITargetDetailInfoBase.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUISkillDetailInfoPassivity.generated.h"

class UTextBlock;
class UPanelWidget;
class UGsUIPassivityDetailInfo;
class UGsButton;

/**
 * 스킬 상세정보 UI - 스킬창 의 패시비티
 */
UCLASS()
class T1PROJECT_API UGsUISkillDetailInfoPassivity : public UGsUITargetDetailInfoBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FGsOnClickActionButton, bool /*bIsAction*/);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPassivityDetailInfo* _detailInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLearn;

public:
	// 인터페이스를 맞추기 위해 사용했고, 무조건 false 리턴
	FGsOnClickActionButton OnClickLearn;

protected:
	FGsIntervalChecker _checkerButton;

public:
	virtual void NativeOnInitialized() override;

public:	
	virtual void SetDataAll(FGsSkillUIData* InData) override;
	virtual void SetData(const FGsSkill* InData, int32 InOverrideLevel = 0) override;
	virtual void SetPassivityData(PassivityId InPassivityId) override;

protected:
	UFUNCTION()
	void OnClickLearnButtton();
};
