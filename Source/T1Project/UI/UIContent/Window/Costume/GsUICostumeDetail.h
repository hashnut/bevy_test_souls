// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUICostumeDetail.generated.h"

class UGsUICostumeActiveSkill;
class UGsUICostumeSkillList;
class UGsUICostumeStat;
class UPanelWidget;

struct FGsCostumeData;

UENUM(BlueprintType)
enum class UseSkillPanel : uint8
{
	NONE = 0 UMETA(Tooltip = "Skill 없음"),
	ActiveSkillDev = 1 UMETA(Tooltip = "Active Skill 분리 사용"),
	AllSkillList = 2 UMETA(Tooltip = "모든 스킬 리스트로 사용"),
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICostumeDetail : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI_Widget")
	UseSkillPanel UseActiveWidget{UseSkillPanel::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI_Widget")
	float WrapSize{ 510.f };

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _identityStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _mainStat1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _mainStat2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _mainStat3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _mainStat4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _mainStat5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _mainStat6;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _subStat1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _subStat2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _subStat3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _subStat4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeStat* _subStat5;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _activeSkillPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _deco;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _skillListPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeActiveSkill* _activeSkill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _title;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSkillTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeSkillList* _skillList;

protected:
	TArray<UGsUICostumeStat*> _itemMainStatList;
	TArray<UGsUICostumeStat*> _itemSubStatList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void SynchronizeProperties() override;
	
public:
	void SetSelectedCostumeDetail(TWeakPtr<FGsCostumeData> data);
	void SetSkillTitle(FText title) { _textSkillTitle = title; };

	CostumeId GetRecoverCostumeId();
};
