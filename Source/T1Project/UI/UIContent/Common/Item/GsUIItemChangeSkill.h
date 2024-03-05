// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIItemChangeSkill.generated.h"

class UImage;
class UPanelWidget;
class UTextBlock;

/**
 * 스킬 전 후 비교 UI
 * - 아이템 단일 강화에서 사용
 */
UCLASS()
class T1PROJECT_API UGsUIItemChangeSkill : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _curSkill;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _nextSkill;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _arrowImage;


public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetCurSkillName(const FText& InSkillName);
	void SetNextSkillName(const FText& InSkillName);
	void SetSkillUIPanel(bool InActive);
	void SetCurSkillNameColor(const FLinearColor& InColor);
	void SetNextSkillNameColor(const FLinearColor& InColor);

	void ShowNextSkillArea(bool InShow);
};
