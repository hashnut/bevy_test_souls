// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "GsUIPopupAbnormalityDetailInfo.generated.h"

class UTextBlock;
class UScrollBox;
class UWidgetSwitcher;
class UGsButton;
class UGsHorizontalBoxIconSelector;
class UGsUIPassivityDetailInfo;
class UGsUISkillDetailInfoEntry;
struct FGsSchemaAbnormalitySet;
struct FGsSchemaPassivitySet;

/**
 * 상태이상 상세 정보창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupAbnormalityDetailInfo : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;
	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textCategory;

	// 0: Abnormality, 1: Passivity
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPassivityDetailInfo* _uiInfoPassivity;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

public:
	void SetAbnormalityData(AbnormalityId InAbnormalityId, int InLevel);
	void SetPassivityData(const struct FGsPassivityData* InData);
	void SetPassivitySet(const struct FGsSchemaPassivitySet* InSchema, PassivityOwnerType InOwnerType);

protected:
	void SetUIStatText(AbnormalityId InAbnormalityId, int InLevel);
	void SetUITimeText(AbnormalityId InAbnormalityId, int InLevel);

	class UGsUIIconAbnormality* GetIcon();

	UFUNCTION()
	void OnClickClose();
};
