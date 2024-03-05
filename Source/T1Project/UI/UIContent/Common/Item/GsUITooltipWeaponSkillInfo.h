// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"

#include "Message/GsMessageContents.h"

#include "GsUITooltipWeaponSkillInfo.generated.h"

class UTextBlock;
struct IGsMessageParam;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITooltipWeaponSkillInfo : public UGsUITooltip
{
	GENERATED_BODY()

protected:
	// 스킬 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleText;

	// '패시브 효과' 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _passiveText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _descText;

private:
	TArray<TPair<MessageContentItemSkill, FDelegateHandle>>	_listItemDelegate;

private:
	// UUserWidget override.
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Touch Click 이 왔을 때 팝업 닫기 / 데이터 
	void BindMessage();
	void UnbindMessage();

private:
	void AckClickedHyperLink(const IGsMessageParam* InParam);

public:
	void SetData(FText&& InTitleText, FText&& InDescText, bool InIsPassive = false);
	void SetTitleColor(const FLinearColor& InColor);
};
