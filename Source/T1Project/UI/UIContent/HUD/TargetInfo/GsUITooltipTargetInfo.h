// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsUITooltipTargetInfo.generated.h"

/**
 * HUD의 타겟UI(GsUIHUDTargetInfo)의 상세정보 클릭 시 뜨는 툴팁
 */
UCLASS()
class T1PROJECT_API UGsUITooltipTargetInfo : public UGsUITooltip
{
	GENERATED_BODY()

protected:
	// 종족
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelRootRace;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textRace;

	// 속성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelRootAttribute;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textAttribute;

	// 재질
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelRootMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMaterial;

	// 패시브
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelRootPassive;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textPassive;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(const struct FGsSchemaNpcData* InData);

protected:
	void SetUIRace(CreatureRaceType InType);
	// 약점 재질
	void SetUIWeaknessMaterial(CreatureRaceType InType);
	// 속성
	void SetUIElement(MonsterElement InType);
};
