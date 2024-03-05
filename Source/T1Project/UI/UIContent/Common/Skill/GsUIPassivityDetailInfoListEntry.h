// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIPassivityDetailInfoListEntry.generated.h"

class FGsSkill;
class UPanelWidget;
class UTextBlock;
class UGsSwitcherButton;
class UGsUISkillDetailInfoEntry;
class UGsUISkillDetailInfoRichEntry;
struct FGsSchemaEffectTextPassivityEffect;

/**
 * 패시비티 effectId 정보
 */
UCLASS()
class T1PROJECT_API UGsUIPassivityDetailInfoListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnOpen;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockConditionOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockConditionOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoCause;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoEffect;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoCoolTime;

protected:
	bool _bHideEmptyCondition = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOpenPanel();

public:
	void SetHideEmptyCondition(bool bInHide);
	void SetData(const FGsSchemaEffectTextPassivityEffect& InData);

	// 전부 무시하고 효과글만 출력. 콜렉션에서 사용
	void SetData(const FString& InStrEffect);

private:
	void SetOpenInfo(bool bIsOpen);
};
