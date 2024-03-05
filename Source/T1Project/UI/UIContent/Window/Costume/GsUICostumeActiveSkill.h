// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Costume/GsCostumeData.h"
#include "GsUICostumeActiveSkill.generated.h"

class UTextBlock;
class UGsButton;
class UGsUISkillIcon;
class UGsUIRedDotBase;

/**
 * 코스튬 페이지 액티브 스킬부분
 */
UCLASS()
class T1PROJECT_API UGsUICostumeActiveSkill : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUICostumeActiveSkill", meta = (BindWidget))
	FLinearColor _colorNoCostume;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUICostumeActiveSkill", meta = (BindWidget))
	FLinearColor _colorCanLearnSkill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUICostumeActiveSkill", meta = (BindWidget))
	FLinearColor _colorCanUseSkill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUICostumeActiveSkill", meta = (BindWidget))
	float _dimmedOpacity = 0.5f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSkillIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillIcon* _uiSkillIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSkillName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSkillState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnShortcut;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDot;

protected:
	SkillId _skillId = INVALID_SKILL_ID;
	CostumeId _costumeId = INVALID_COSTUME_ID;
	CostumeId _recoverCostumeId = INVALID_COSTUME_ID;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickSkillIcon();
	UFUNCTION()
	void OnClickShortcut();

public:
	void SetData(TWeakPtr<FGsCostumeData> costumeData);

protected:
	bool SetDataInter(TWeakPtr<FGsCostumeData> costumeData);
	void SetUISkillState(bool bIsCollected, bool bIsLearn);

public:
	CostumeId GetRecoverCostumeId();
};
